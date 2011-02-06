#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>

#include "dirent.h"

#include "bpn.h"

BPN *bpn0;

void print_arguments() {
  std::cout << "Usage: ntrain-cli [create|train|test|convert] log_directory/bpn_file [bpn_file] [threads] [epochs]" << std::endl
	    << "--------------------------------------------------------------------------------------------------" << std::endl
	    << "ntrain-cli create bpn_file" << std::endl
	    << "  Create and save a new network to `bpn_file'." << std::endl << std::endl
	    << "ntrain-cli train log_directory bpn_file [threads=1] [epochs=1]" << std::endl
	    << "  Train network `bpn_file' (if doesn't exist create a new one) over `log_directory'" << std::endl
	    << "  `epoch' number of times with `threads' threads and save to `bpn_file'." << std::endl << std::endl
	    << "ntrain-cli test log_directory bpn_file [threads=1]" << std::endl
	    << "  Test network `bpn_file' (if doesn't exist create a new one) over `log_directory'" << std::endl
	    << "  with `threads' threads." << std::endl << std::endl
	    << "ntrain-cli convert log_directory" << std::endl
	    << "  Convert log files in directory `log_directory' to bpnl format" << std::endl
	    << "  keeping log files as well." << std::endl;
}

bool ends_with (const char *str, const char *end) {
  unsigned len_str = strlen(str), len_end = strlen(end);

  if (len_str < len_end) return false;
  return strcmp((const char*) str + len_str - len_end, end) ==0;
}

int train (const char *bpn_file, const std::string folder, unsigned threads, unsigned epochs) {
  try {
    bpn0 = new BPN(bpn_file, threads -1);

    DIR *dir;
    struct dirent *ent;
    dir = opendir(folder.c_str());

    if (dir != NULL) {
      unsigned logs = 0, bpnls = 0;

      while ((ent = readdir(dir)) != NULL) {
	if (ends_with(ent->d_name, ".bpnl")) {
	  ++bpnls;
	}
	else if (ends_with(ent->d_name, ".log"))
	  ++logs;
      }

      rewinddir(dir);

      time_t rawtime;
      time(&rawtime);

      std::cout << "Start training over ";

      if (bpnls >0) {		// .bpnl
	std::cout << bpnls << " bpnl files "  << ctime(&rawtime);

	while ((ent = readdir(dir)) != NULL) {
	  if (ends_with(ent->d_name, ".bpnl")) {
	    std::cout << "Training over " << ent->d_name << ": ";

	    for (unsigned i=0; i < epochs; ++i) {
	      if(bpn0->TrainOverFile(folder + ent->d_name)) {
		bpn0->SaveToFile((char*)"./bpn~");
	      }
	      else {
		delete bpn0;
		bpn0 = new BPN((char*)"./bpn~", threads -1);
		std::cout << "error ";
	      }

	      time(&rawtime);
	      std::cout << ctime(&rawtime);
	    }
	  }
	}
      }
      else {			// .log
	std::cout << logs << " log files " << ctime(&rawtime);

	while ((ent = readdir(dir)) != NULL) {
	  if (ends_with(ent->d_name, ".log")) {
	    std::cout << "Training over " << ent->d_name << ": ";

	    for (unsigned i=0; i < epochs; ++i) {
	      if(bpn0->TrainOverRawFile(folder + ent->d_name)) {
		bpn0->SaveToFile((char*)"./bpn~");
	      }
	      else {
		delete bpn0;
		bpn0 = new BPN((char*)"./bpn~", threads -1);
		std::cout << "error ";
	      }

	      time(&rawtime);
	      std::cout << ctime(&rawtime);
	    }
	  }
	}
      }

      closedir (dir);
    }
    else {
      std::cout << "Not a valid directory: " << folder << std::endl;
      return 2;
    }
  }
  catch (std::exception ex) {
    std::cout << ex.what() << std::endl;
    return 1;
  }

  return 0;
}

int test (const char *bpn_file, const std::string folder, unsigned threads) {
  try {
    bpn0 = new BPN(bpn_file, threads -1);

    DIR *dir;
    struct dirent *ent;
    dir = opendir (folder.c_str());

    if (dir != NULL) {
      unsigned logs = 0, bpnls = 0;

      while ((ent = readdir(dir)) != NULL) {
	if (ends_with(ent->d_name, ".bpnl")) {
	  ++bpnls;
	}
	else {
	  if (ends_with(ent->d_name, "log")) ++logs;
	}
      }

      rewinddir(dir);

      double total_error = 0, current_error;
      int total_count=0, i=0;

      time_t rawtime;
      time(&rawtime);

      std::cout << "Start testing over ";

      if (bpnls >0) {		// .bpnl
	std::cout << bpnls << " bpnl files "  << ctime(&rawtime);

	while ((ent = readdir(dir)) != NULL) {
	  if (ends_with(ent->d_name, "bpnl")) {
	    i = 0;

	    current_error = bpn0->TestOverFile(folder + ent->d_name, i);

	    if(i > 0) {
	      total_error += current_error;
	      total_count += i;

	      current_error /= (float)i;
	    }

	    std::cout << "Mean error over " << ent->d_name << ": " << current_error << std::endl;

	  }
	}
      }
      else {			// .log
	std::cout << logs << " log files " << ctime(&rawtime);

	while ((ent = readdir(dir)) != NULL) {
	  if (ends_with(ent->d_name, ".log")) {
	    i = 0;

	    current_error = bpn0->TestOverRawFile(folder + ent->d_name, i);

	    if(i > 0) {
	      total_error += current_error;
	      total_count += i;

	      current_error /= (float)i;
	    }

	    std::cout << "Mean error over " << ent->d_name << ": " << current_error << std::endl;
	  }
	}
      }

      if(total_count > 0) total_error /= (float)total_count;

      std::cout << "Total mean error: " << total_error << std::endl;

      time(&rawtime);
      std::cout << ctime(&rawtime);

      closedir (dir);
    }
    else {
      std::cout << "Not a valid directory: " << folder << std::endl;
      return 2;
    }
  }
  catch (std::exception ex) {
    std::cout << ex.what() << std::endl;
    return 1;
  }

  return 0;
}

void FENtoInput(const char *line, std::ofstream &out) {
  unsigned i=0, j=0, len = strlen(line);

  while(i<256 && j < len) {	//	this is for 64 squares
    switch(line[j]) {
    case 'P':	//	white pawn - code 0001
      out<<"0001";
      i +=4;
      break;
    case 'p':	//	black pawn - code 1001
      out<<"1001";
      i +=4;
      break;
    case 'N':	//	white kNight - code 0010
      out<<"0010";
      i +=4;
      break;
    case 'n':	//	black kNight - code 1010
      out<<"1010";
      i +=4;
      break;
    case 'B':	//	white bishop - code 0011
      out<<"0011";
      i +=4;
      break;
    case 'b':	//	black bishop - code 1011
      out<<"1011";
      i +=4;
      break;
    case 'R':	//	white rook - code 0100
      out<<"0100";
      i +=4;
      break;
    case 'r':	//	black rook - code 1100
      out<<"1100";
      i +=4;
      break;
    case 'Q':	//	white queen - code 0101
      out<<"0101";
      i +=4;
      break;
    case 'q':	//	black queen - code 1101
      out<<"1101";
      i +=4;
      break;
    case 'K':	//	white king - code 0110
      out<<"0110";
      i +=4;
      break;
    case 'k':	//	black king - code 1110
      out<<"1110";
      i +=4;
      break;
    }

    if(line[j] <= '8' && line[j] >= '1') {
      int emptySquares = line[j] - '0';
      emptySquares *= 4;

      for(int k=0; k< emptySquares; ++k) {
	//out<<true;
	out<<'1';
      }

      i += emptySquares;
    }

    ++j;
  }

  while(line[j] == ' ')
    ++j;

  if(line[j] == 'w' || line[j] == 'W') out<<'1';	//	set active colour bit - i == 256
  else out<<'0';

  do {
    ++j;
  }while(line[j] == ' ');

  char flags[4] =  {'0','0','0','0'};

  while(line[j] != '-' && line[j] != ' ') {
    switch(line[j]) {
    case 'K':	//	White can castle kingside
      flags[0] = '1';
      break;
    case 'Q':	//	White can castle queenside
      flags[1] = '1';
      break;
    case 'k':	//	Black can castle kingside
      flags[2] = '1';
      break;
    case 'q':	//	Black can castle queenside
      flags[3] = '1';
      break;
    }

    ++j;
  }

  for(i=0; i<4; ++i) {
    out<<flags[i];
  }

  while(line[j] == ' ')
    ++j;

  if(line[j] =='-')
    out<<'0';
  else    //  en passant possible
    out<<'1';

  while(line[j] != '[')
    ++j;

  ++j;
  out << " ";

  while (line[j] != ']')
    out << line[j++];
}

void convert_file(const std::string file) {
  std::string new_file = file.substr(0, file.length() - 3) + "bpnl";
  std::ifstream fin;
  std::ofstream fout;

  fin.open(file.c_str(), std::ifstream::in);
  if(!fin.is_open()) return;

  fout.open(new_file.c_str(), std::ofstream::out);
  if(!fout.is_open()) return;

  std::string str;
  while(getline(fin, str)) {
    FENtoInput(str.c_str(), fout);
    fout << std::endl;
  }

  fout.close();
  fin.close();
}

int convert(std::string folder) {
  DIR *dir;
  struct dirent *ent;
  dir = opendir (folder.c_str());

  if (dir != NULL) {
    unsigned logs = 0;

    while ((ent = readdir(dir)) != NULL) {
      if (ends_with(ent->d_name, ".log")) ++logs;
    }

    rewinddir(dir);

    time_t rawtime;
    time(&rawtime);

    std::cout << "Start converting " << logs << " files: " << ctime(&rawtime);

    while ((ent = readdir(dir)) != NULL) {
      if (ends_with(ent->d_name, ".log")) {
	std::cout << folder + ent->d_name << " ..." << std::endl;
	convert_file(folder + ent->d_name);
      }
    }

    time(&rawtime);
    std::cout << "End of converting " << ctime(&rawtime);

    closedir (dir);
  }
  else {
    std::cout << "Not a valid directory: " << folder << std::endl;
    return 2;
  }

  return 0;
}

int main(int argc, char * argv[]) {
  if (argc < 3) {
    print_arguments();
    return 0;
  }

  if (strcmp(argv[1], "create") ==0) {
    bpn0 = new BPN();
    bpn0->SaveToFile(argv[2]);
    delete bpn0;

    return 0;
  }

  unsigned threads = 1, epochs = 1;

  if (argc > 4) {
    try {
      threads = atof(argv[4]);

      if (argc > 5) epochs = atof(argv[5]);
    }
    catch (std::exception ex) {
      print_arguments();
      std::cout << ex.what() << std::endl;

      return 1;
    }
  }

  std::string dir = std::string(argv[2]);
  if (!ends_with(dir.c_str(), "/"))
    dir += "/";

  if (strcmp(argv[1], "convert") ==0)
    return convert(dir);
  else if (strcmp(argv[1], "train") ==0) {
    if (train(argv[3], dir, threads, epochs) ==0) {
      bpn0->SaveToFile(argv[3]);
      delete bpn0;

      return 0;
    }
    else
      return 1;
  }
  else if (strcmp(argv[1], "test") ==0) {
    int ret = test(argv[3], dir, threads);
    delete bpn0;

    return ret;
  }
  else
    print_arguments();

  return 0;
}
