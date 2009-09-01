#include <fstream>
#include <cstdlib>
#include <cmath>
#include "bpn.h"
#include "bpnlayer.h"

BPN::~BPN()
{
    if(size >0)
    {
        if(layers[size-1]->size >0)
            delete [] train_output;

        for(unsigned i=0; i<size; ++i)
        {
            delete layers[i];
        }

        delete [] layers;
    }
}

BPN::BPN(unsigned *sizes, bool *biases, outFunction *functions, unsigned layersNumber, double et, double alph, double o_factor)
{
    size = layersNumber;
    alpha = alph;
    eta = et;
    scale_factor = o_factor;
    initial_scale = 0.03f;

    if(size >0)
    {
        layers = new bpnLayer*[size];

        layers[0] = new bpnLayer(sizes[0], 0, false, functions[0]);   //  input layer - no lower layer

        for(unsigned i=1; i<size; ++i)
        {
            layers[i] = new bpnLayer(sizes[i], sizes[i-1], biases[i], functions[i]);
        }

        if(layers[size-1]->size >0)
        {
            train_output = new double[layers[size-1]->size];
            for(unsigned j=0; j <layers[size-1]->size; ++j)
            {
                train_output[j] = 0.0f;
            }
        }

        InitializeWeights();
    }
}

BPN::BPN(char* file)
{
    std::ifstream fin;
    //std::string ignore;
    fin.open(file, std::ifstream::in);

    //ignore = readString(fin);
    readString(fin);
    fin>>initial_scale;

    //ignore = readString(fin);
    readString(fin);
    fin>>eta;

    //ignore = readString(fin);
    readString(fin);
    fin>>alpha;

    //ignore = readString(fin);
    readString(fin);
    fin>>scale_factor;

    //ignore = readString(fin);
    readString(fin);
    fin>>size;

    if(size >0)
    {
        layers = new bpnLayer*[size];

        unsigned i, f, layersize, prevsize=0;
        bool bias;

        //  input layer - no bias
        //ignore = readString(fin);
        //ignore = readString(fin);
        readString(fin);
        readString(fin);
        fin>>layersize;

        //ignore = readString(fin);
        readString(fin);
        fin>>bias;

        //ignore = readString(fin);
        readString(fin);
        fin>>f;

        bias = false;
        layers[0] = new bpnLayer(layersize, prevsize, bias, (outFunction)f);
        prevsize = layersize;

        for(i=1; i<size; ++i)   //  itterate all layer above input
        {
            //ignore = readString(fin);
            //ignore = readString(fin);
            readString(fin);
            readString(fin);
            fin>>layersize;

            //ignore = readString(fin);
            readString(fin);
            fin>>bias;

            //ignore = readString(fin);
            readString(fin);
            fin>>f;

            layers[i] = new bpnLayer(layersize, prevsize, bias, (outFunction)f);
            prevsize = layersize;
        }

        if(layers[size-1]->size >0)
        {
            train_output = new double[layers[size-1]->size];
            for(i=0; i <layers[size-1]->size; ++i)
            {
                train_output[i] = 0.0f;
            }
        }

        char ch;
        fin>>ch;

        if(ch =='x')
        {
            fin.close();
            InitializeWeights();
            return;
        }

        for(i=0; i<size; ++i)
        {
            LoadLayer(layers[i], fin);
        }
    }

    fin.close();
}

void BPN::LoadLayer(bpnLayer* layer, std::ifstream& fin)
{
    unsigned i, j;

    if(layer->bias)
    {
        for(i=0; i<layer->size; ++i)
        {
            for(j=0; j<layer->lowerSize; ++j)
            {
                fin>>layer->weights[i][j];
            }

            fin>>layer->biases[i];
        }
    }
    else
    {
        for(i=0; i<layer->size; ++i)
        {
            for(j=0; j<layer->lowerSize; ++j)
            {
                fin>>layer->weights[i][j];
            }
        }
    }
}

std::string BPN::readString(std::ifstream& fin)
{
    char ch;
    std::string acc="";

    fin.get(ch);

    do{
        fin.get(ch);;

        if(ch != ' ')
            acc += ch;
    }
    while(ch != ' ');

    return acc;
}

bool BPN::SaveToFile(char* file)
{
    unsigned i;
    std::ofstream fout;

    try
    {
        fout.open(file, std::ofstream::out);

        fout<<" init_scale: "<<initial_scale<<" eta: "<<eta<<" alpha: "<<alpha<<" scale_factor: "<<scale_factor<<" layers_num: "<<size;

        for(i=0; i<size; ++i)
        {
            fout<<" layer"<<i+1<<": size "<<layers[i]->size<<" bias "<<layers[i]->bias<<" func "<<(int)layers[i]->func;
        }

        fout<<"i\n";
    }
    catch(std::exception ex)
    {
        return false;
    }

    for(i=0; i<size; ++i)
    {
        if(!SaveLayer(layers[i], fout))
        {
            fout.close();
            return false;
        }
    }

    fout.close();
    return true;
}

bool BPN::SaveLayer(bpnLayer* layer, std::ofstream& fout)
{
    unsigned i, j;

    if(layer->bias)
    {
        for(i=0; i<layer->size; ++i)
        {
            try
            {
                for(j=0; j<layer->lowerSize; ++j)
                {
                    fout<<layer->weights[i][j]<<" ";
                }

                fout<<layer->biases[i]<<" "<<"\n";
            }
            catch(std::exception ex)
            {
                return false;
            }
        }
    }
    else
    {
        for(i=0; i<layer->size; ++i)
        {
            for(j=0; j<layer->lowerSize; ++j)
            {
                try
                {
                    fout<<layer->weights[i][j]<<" ";
                }
                catch(std::exception ex)
                {
                    return false;
                }
            }

            fout<<"\n";
        }
    }

    return true;
}

void BPN::InitializeWeights()
{
    unsigned i, j, k, hiddenNum =0;

    for(i=1; i<size-1; ++i)
    {
        hiddenNum += layers[i]->size;
    }

//    double scale = (double) (pow((double) (0.7f * (double) hiddenNum),
//                                  (double) (1.0f / (double) layers[0]->size)))/scale_factor;
//
//    scale = 0.03f;

    for(i=1; i<size; ++i)   //  itterate layers above input
    {
        if(layers[i]->bias)
        {
            for(j=0; j<layers[i]->size; ++j)
            {
                for(k=0; k<layers[i]->lowerSize; ++k)
                {
                    layers[i]->weights[j][k] = randomNum(-initial_scale, initial_scale);
                }

                layers[i]->biases[j] = randomNum(-initial_scale, initial_scale);
            }
        }
        else
        {
            for(j=0; j<layers[i]->size; ++j)
            {
                for(k=0; k<layers[i]->lowerSize; ++k)
                {
                    layers[i]->weights[j][k] = randomNum(-initial_scale, initial_scale);
                }
            }
        }
    }
}

bool BPN::Train(double *input, double* output)
{
    Run(input);

    double (BPN::*derivate) (double); //  pointer to derivate funtion

    switch(layers[size-1]->func)
    {
        case sigmoid:
            derivate = &BPN::DerivateSigmoid;
            break;
        case sigmoid2:
            derivate = &BPN::DerivateSigmoid2;
            break;
        default:
            derivate = &BPN::DerivateLinear;
    }

    for(unsigned i=0; i<layers[size -1]->size; ++i)  //  fill output errors
    {
        layers[size -1]->errors[i] = (double)(this->*derivate)(layers[size -1]->products[i]/scale_factor)*(output[i] - layers[size -1]->products[i])/scale_factor;

        for(unsigned j=0; j<layers[size-2]->size; ++j)
        {
            layers[size -1]->deltas[i][j] = layers[size-2]->products[j]*eta*layers[size-1]->errors[i] + alpha*layers[size -1]->deltas[i][j];
        }
    }

    return Train();
}

void BPN::Run(double* input)
{
    for(unsigned i=0; i<layers[0]->size; ++i)    //  fill input
    {
        layers[0]->products[i] = input[i];
    }

    Run();
}

void BPN::PrepareFromFEN(char* fen)
{
    int i=0;
    int j=0;
    char ch;

    while(i<256 && j<100)	//	this is for 64 squares
    {
        ch = fen[j];

        switch(ch)
        {
        case 'P':	//	white pawn - code 0001
            layers[0]->products[i] = 0.0f;
            layers[0]->products[i+1] = 0.0f;
            layers[0]->products[i+2] = 0.0f;
            layers[0]->products[i+3] = 1.0f;
            i = i + 4;
            break;
        case 'p':	//	black pawn - code 1001
            layers[0]->products[i] = 1.0f;
            layers[0]->products[i+1] = 0.0f;
            layers[0]->products[i+2] = 0.0f;
            layers[0]->products[i+3] = 1.0f;
            i = i + 4;
            break;
        case 'N':	//	white kNight - code 0010
            layers[0]->products[i] = 0.0f;
            layers[0]->products[i+1] = 0.0f;
            layers[0]->products[i+2] = 1.0f;
            layers[0]->products[i+3] = 0.0f;
            i = i + 4;
            break;
        case 'n':	//	black kNight - code 1010
            layers[0]->products[i] = 1.0f;
            layers[0]->products[i+1] = 0.0f;
            layers[0]->products[i+2] = 1.0f;
            layers[0]->products[i+3] = 0.0f;
            i = i + 4;
            break;
        case 'B':	//	white bishop - code 0011
            layers[0]->products[i] = 0.0f;
            layers[0]->products[i+1] = 0.0f;
            layers[0]->products[i+2] = 1.0f;
            layers[0]->products[i+3] = 1.0f;
            i = i + 4;
            break;
        case 'b':	//	black bishop - code 1011
            layers[0]->products[i] = 1.0f;
            layers[0]->products[i+1] = 0.0f;
            layers[0]->products[i+2] = 1.0f;
            layers[0]->products[i+3] = 1.0f;
            i = i + 4;
            break;
        case 'R':	//	white rook - code 0100
            layers[0]->products[i] = 0.0f;
            layers[0]->products[i+1] = 1.0f;
            layers[0]->products[i+2] = 0.0f;
            layers[0]->products[i+3] = 0.0f;
            i = i + 4;
            break;
        case 'r':	//	black rook - code 1100
            layers[0]->products[i] = 1.0f;
            layers[0]->products[i+1] = 1.0f;
            layers[0]->products[i+2] = 0.0f;
            layers[0]->products[i+3] = 0.0f;
            i = i + 4;
            break;
        case 'Q':	//	white queen - code 0101
            layers[0]->products[i] = 0.0f;
            layers[0]->products[i+1] = 1.0f;
            layers[0]->products[i+2] = 0.0f;
            layers[0]->products[i+3] = 0.0f;
            i = i + 4;
            break;
        case 'q':	//	black queen - code 1101
            layers[0]->products[i] = 1.0f;
            layers[0]->products[i+1] = 1.0f;
            layers[0]->products[i+2] = 0.0f;
            layers[0]->products[i+3] = 1.0f;
            i = i + 4;
            break;
        case 'K':	//	white king - code 0110
            layers[0]->products[i] = 0.0f;
            layers[0]->products[i+1] = 1.0f;
            layers[0]->products[i+2] = 1.0f;
            layers[0]->products[i+3] = 0.0f;
            i = i + 4;
            break;
        case 'k':	//	black king - code 1110
            layers[0]->products[i] = 1.0f;
            layers[0]->products[i+1] = 1.0f;
            layers[0]->products[i+2] = 1.0f;
            layers[0]->products[i+3] = 0.0f;
            i = i + 4;
            break;
        }

        if(ch <= '8' && ch>= '1')
        {
            int emptySquares = ch - '0';
            emptySquares *= 4;

            for(int k=0; k< emptySquares; ++k)
            {
                layers[0]->products[i] = 1.0f;
                ++i;
            }
        }

        ++j;
    }

    ch = fen[j];
    while(ch == ' ')
    {
        ++j;
        ch = fen[j];
    }

    if(ch == 'w' || ch == 'W') layers[0]->products[256] = 1.0f;	//	set active colour bit - i == 256
    else layers[0]->products[256] = 0.0f;

    do{
        ++j;
        ch = fen[j];
    }while(ch == ' ');

    for(i=257; i<261; ++i)
        layers[0]->products[i] = 0.0f;

    while(ch != '-' && ch != ' ')
    {
        switch(ch)
        {
        case 'K':	//	White can castle kingside
                        layers[0]->products[257] = 1.0f;
                break;
        case 'Q':	//	White can castle queenside
                        layers[0]->products[258] = 1.0f;
                break;
        case 'k':	//	Black can castle kingside
                        layers[0]->products[259] = 1.0f;
                break;
        case 'q':	//	Black can castle queenside
                        layers[0]->products[260] = 1.0f;
                break;
        }

        ++j;
        ch = fen[j];
    }

    while(fen[j] == ' ')
            ++j;

    if(fen[j] =='-')
        layers[0]->products[261] = 0.0f;
    else    //  en passant possible
        layers[0]->products[261] = 1.0f;

    while(fen[j] != '[')
            ++j;

    train_output[0] = atof((char*)(fen + j + 1));
}

void BPN::Run() //  assume that input is already placed in the first layer
{
    unsigned i, j, k;
    double net;
    double (BPN::*apply) (double); //  pointer to output funtion

    for(i=1; i<size-1; ++i)  //  for each layer above input
    {
        //  determine output function before iterating through units - it's always same function for the whole layer
        switch(layers[i]->func)
        {
            case sigmoid:
                apply = &BPN::ApplySigmoid;
                break;
            case sigmoid2:
                apply = &BPN::ApplySigmoid2;
                break;
            default:
                apply = &BPN::ApplyLinear;
        }

        if(layers[i]->bias)
        {
            for(j=0; j<layers[i]->size; ++j)   //  for each unit - computate product
            {
                net = 0;

                for(k=0; k<layers[i]->lowerSize; ++k)
                {
                    net += (double)layers[i]->weights[j][k]*layers[i-1]->products[k];
                }

                net += layers[i]->biases[j];

                layers[i]->products[j] = (this->*apply)(net);
            }
        }
        else
        {
            for(j=0; j<layers[i]->size; ++j)   //  for each unit - compute product
            {
                net = 0;

                for(k=0; k<layers[i]->lowerSize; ++k)
                {
                    net += layers[i]->weights[j][k]*layers[i-1]->products[k];
                }

                layers[i]->products[j] = (this->*apply)(net);
            }
        }
    }

    //  same for output layer - though add scale factor
    switch(layers[size-1]->func)
    {
        case sigmoid:
            apply = &BPN::ApplySigmoid;
            break;
        case sigmoid2:
            apply = &BPN::ApplySigmoid2;
            break;
        default:
            apply = &BPN::ApplyLinear;
    }

    if(layers[size-1]->bias)
    {
        for(j=0; j<layers[size-1]->size; ++j)   //  for each unit - computate product
        {
            net = 0;

            for(k=0; k<layers[size-1]->lowerSize; ++k)
            {
                net += (double)layers[size-1]->weights[j][k]*layers[size-2]->products[k];
            }

            net += layers[size-1]->biases[j];

            layers[size-1]->products[j] = (this->*apply)(net)*scale_factor;
        }
    }
    else
    {
        for(j=0; j<layers[size-1]->size; ++j)   //  for each unit - computate product
        {
            net = 0;

            for(k=0; k<layers[size-1]->lowerSize; ++k)
            {
                net += (double)layers[size-1]->weights[j][k]*layers[size-2]->products[k];
            }

            layers[size-1]->products[j] = (this->*apply)(net)*scale_factor;
        }
    }
}

bool BPN::Train(char* fen)
{
    Run(fen);

    double (BPN::*derivate) (double); //  pointer to derivate funtion

    switch(layers[size-1]->func)
    {
        case sigmoid:
            derivate = &BPN::DerivateSigmoid;
            break;
        case sigmoid2:
                derivate = &BPN::DerivateSigmoid2;
                break;
        default:
            derivate = &BPN::DerivateLinear;
    }

    for(unsigned i=0; i<layers[size -1]->size; ++i)  //  fill output errors
    {
        layers[size -1]->errors[i] = (double)(this->*derivate)(layers[size -1]->products[i]/scale_factor)*(train_output[i] - layers[size -1]->products[i])/scale_factor;

        for(unsigned j=0; j<layers[size-2]->size; ++j)
        {
            layers[size -1]->deltas[i][j] = layers[size-2]->products[j]*eta*layers[size-1]->errors[i] + alpha*layers[size-1]->deltas[i][j];
        }
    }

    return Train();
}

bool BPN::Train()
{
    unsigned i, j, k;
    double err;

    double (BPN::*derivate) (double); //  pointer to derivate funtion

    for(i=size-2; i>0; --i)    //  compute errors and weights' changes - itterate layers from top to bottom
    {
        //  determine derivate function before iterating through units - it's always same derivate for the whole layer
        switch(layers[i]->func)
        {
            case sigmoid:
                derivate = &BPN::DerivateSigmoid;
                break;
            case sigmoid2:
                derivate = &BPN::DerivateSigmoid2;
                break;
            default:
                derivate = &BPN::DerivateLinear;
        }

        for(j=0; j<layers[i]->size; ++j)   //  for each unit in the layer
        {
            err = 0.0f;

            try
            {
                for(k=0; k<layers[i+1]->size; ++k)    //  for each unit in the upper layer
                {
                    err += layers[i+1]->errors[k]*layers[i+1]->weights[k][j];
                }

                layers[i]->errors[j] = (double)(this->*derivate)(layers[i]->products[j])*err;  //  compute error

                for(k=0; k<layers[i-1]->size; ++k)
                {
                    layers[i]->deltas[j][k] = layers[i-1]->products[k]*eta*layers[i]->errors[j] + alpha*layers[i]->deltas[j][k];
                    //layers[i]->weights[j][k] += layers[i]->deltas[j][k];  //  can't do it here because of the lower layer errors
                }
            }
            catch(std::exception ex)
            {
                return false;
            }
        }
    }

    try
    {
        for(i=1; i<size; ++i)   //  for each layer (above input)
        {
            for(j=0; j<layers[i]->size; ++j)    //  for each unit
            {
                for(k=0; k<layers[i]->lowerSize; ++k)   //  for each weight - renew
                {
                    layers[i]->weights[j][k] += (double)layers[i]->deltas[j][k];
                    if(std::isnan(layers[i]->weights[j][k]))
                        return false;
                }
            }
        }
    }
    catch(std::exception ex)
    {
        return false;
    }

    return true;
}

void BPN::Run(char* fen)
{
    PrepareFromFEN(fen);
    Run();
}

double BPN::ApplyFunction(outFunction func, double net)
{
    switch(func)
    {
        case sigmoid:
            net = (double)1/(1+exp(-net));
            break;
        case sigmoid2:
            net = (double)2 / (1 + exp(-2 * net)) - 1;
            break;
        default:
            break;
    }

    return net;
}

double BPN::ApplyDerivate(outFunction func, double val)
{
    switch(func)
    {
        case sigmoid:
            val = val*(1-val);
            break;
        case sigmoid2:
            val = (double) 1 - (pow(val, 2));
            break;
        default:
            val = 1;
            break;
    }

    return val;
}

double BPN::ApplyLinear(double val)
{
    return val;
}

double BPN::ApplySigmoid(double val)
{
    return (double)1/(1+exp(-val));
}

double BPN::ApplySigmoid2(double val)
{
    return (double)2 / (1 + exp(-2 * val)) - 1;
}

double BPN::DerivateLinear(double val)
{
    return 1;
}

double BPN::DerivateSigmoid(double val)
{
    return val*(1-val);
}

double BPN::DerivateSigmoid2(double val)
{
   return (double) 1 - (pow(val, 2));
}

double BPN::randomNum(double minv, double maxv)
{
    return (double)minv + (maxv - minv)*rand()/(RAND_MAX+1.0f);
}
