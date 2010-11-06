#include <QFileDialog>
#include <QTextStream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "ntrain.h"
#include "ui_ntrain.h"
#include "bpn.h"

nTrain::nTrain(QWidget *parent)
  : QDialog(parent), ui(new Ui::nTrainClass) {
  ui->setupUi(this);
  ui->pbProgress->setVisible(false);
  ui->teLog->ensureCursorVisible();

  ConstructBPN();
}

nTrain::~nTrain() {
  delete bpn0;
  delete ui;
}

void nTrain::on_pbTrain_clicked() {
  QDir logDir(ui->leTrainPath->text(), "*.bpnl", QDir::Name, QDir::Files);

  if(!logDir.exists()) return;

  QCursor wait(Qt::WaitCursor);
  QApplication::setOverrideCursor(wait);

  time_t rawtime;

  ui->teLog->append("Starting training ");
  time ( &rawtime );
  ui->teLog->append(ctime(&rawtime));
  //ui->teLog->scroll(0,50);
  ui->teLog->repaint();

  ui->pbProgress->reset();

  if(logDir.entryInfoList().count() >0) {
    ui->pbProgress->setRange(0, logDir.entryInfoList().count()*ui->sbEpochs->value());
    ui->pbProgress->setValue(0);
    ui->pbProgress->setVisible(true);

    foreach(QFileInfo log, logDir.entryInfoList()) {
      ui->teLog->append("Training over " + log.baseName());
      //ui->teLog->scroll(0,50);
      ui->teLog->repaint();

      for(int i=0; i < ui->sbEpochs->value(); ++i) {
	if(TrainOverFile(log.absoluteFilePath())) {
	  bpn0->SaveToFile((char*)"./bpn~");
	}
	else {
	  delete bpn0;
	  bpn0 = new BPN((char*)"./bpn~");

	  ui->teLog->append("error");
	  ui->teLog->repaint();
	}

	time ( &rawtime );

	ui->teLog->append(ctime(&rawtime));
	//ui->teLog->scroll(0,50);
	ui->teLog->repaint();

	ui->pbProgress->setValue(ui->pbProgress->value() + 1);
      }
    }
  }
  else {
    QStringList logFilter("*.log");
    logDir.setNameFilters(logFilter);

    ui->pbProgress->setRange(0, logDir.entryInfoList().count()*ui->sbEpochs->value());
    ui->pbProgress->setValue(0);
    ui->pbProgress->setVisible(true);

    foreach(QFileInfo log, logDir.entryInfoList()) {
      ui->teLog->append("Training over " + log.baseName());
      //ui->teLog->scroll(0,50);
      ui->teLog->repaint();

      for(int i=0; i < ui->sbEpochs->value(); ++i) {
	if(TrainOverRawFile(log.absoluteFilePath())) {
	  bpn0->SaveToFile((char*)"./bpn~");
	}
	else {
	  delete bpn0;
	  bpn0 = new BPN((char*)"./bpn~");

	  ui->teLog->append("error");
	  ui->teLog->repaint();
	}

	time ( &rawtime );

	ui->teLog->append(ctime(&rawtime));
	//ui->teLog->scroll(0,50);
	ui->teLog->repaint();

	ui->pbProgress->setValue(ui->pbProgress->value() + 1);
      }
    }
  }

  ui->pbProgress->setVisible(false);
  ui->teLog->append("End of training.");

  QApplication::restoreOverrideCursor();
}

bool nTrain::TrainOverFile(QString filePath) {
  QFile filelog(filePath);
  if (!filelog.open(QIODevice::ReadOnly | QIODevice::Text))
    return false;

  QTextStream in(&filelog);

  double input[262];
  double output[1];

  while(!in.atEnd()) {
    char* line = in.readLine().toLatin1().data();

    for(int i=0; i<262; ++i) {
      input[i] = (double)((int)line[i] - 48);
    }

    output[0] = atof((char*)(line + 263));

    if(!bpn0->Train(input, output)) {
      filelog.close();
      return false;
    }
  }

  filelog.close();
  return true;
}

bool nTrain::TrainOverRawFile(QString filePath) {
  QFile filelog(filePath);
  if (!filelog.open(QIODevice::ReadOnly | QIODevice::Text))
    return false;

  QTextStream in(&filelog);

  while(!in.atEnd()) {
    char* line = in.readLine().toLatin1().data();
    if(!bpn0->Train(line)) {
      filelog.close();
      return false;
    }
  }

  filelog.close();
  return true;
}

void nTrain::on_pbBrowseDir_clicked() {
  QString folder = QFileDialog::getExistingDirectory(this, tr("Locate Train Folder"), ".");
  if(!folder.isEmpty())
    ui->leTrainPath->setText(folder);
}

void nTrain::ConstructBPN() {
  unsigned sizes[] = {262, 66, 256, 1};
  outFunction functions[] = {sigmoid2, sigmoid2, sigmoid2, sigmoid2};
  bool biases[] = {false, true, true, true};

  bpn0 = new BPN(sizes, biases, functions, 4, 0.35f, 0.3f, 29744.0f);
  bpn0->InitializeWeights();
}

void nTrain::on_pbBrowseFile_clicked() {
  QString file = QFileDialog::getOpenFileName(this, tr("Choose bpn file"), ui->leOpenPath->text());
  if(!file.isEmpty()) {
    ui->leOpenPath->setText(file);

    delete bpn0;

    QCursor wait(Qt::WaitCursor);
    QApplication::setOverrideCursor(wait);

    try {
      bpn0 = new BPN(file.toLatin1().data());
      ui->teLog->append(ui->leOpenPath->text() + " successfully loaded.");
    }
    catch(std::exception ex) {
      ui->teLog->append("Filed to load " + ui->leOpenPath->text());
    }

    QApplication::restoreOverrideCursor();
  }
}

void nTrain::on_pbSave_clicked() {
  QString file = QFileDialog::getSaveFileName(this,
					      tr("Save bpn file"), ui->leOpenPath->text());

  ui->leOpenPath->setText(file);

  QCursor wait(Qt::WaitCursor);
  QApplication::setOverrideCursor(wait);

  if(bpn0->SaveToFile(file.toLatin1().data()))
    ui->teLog->append(ui->leOpenPath->text() + " successfully saved.");
  else
    ui->teLog->append("Filed to save to " + ui->leOpenPath->text());

  QApplication::restoreOverrideCursor();
}

void nTrain::on_pbConvert_clicked() {
  QDir logDir(ui->leTrainPath->text(), "*.log", QDir::NoSort, QDir::Files);

  if(!logDir.exists()) return;

  QCursor wait(Qt::WaitCursor);
  QApplication::setOverrideCursor(wait);

  ui->pbProgress->setRange(0, logDir.entryInfoList().count()*ui->sbEpochs->value());
  ui->pbProgress->setValue(0);
  ui->pbProgress->setVisible(true);

  foreach(QFileInfo log, logDir.entryInfoList()) {
    ui->teLog->append("Converting " + log.baseName() + "...");
    ui->teLog->repaint();
    ConvertLog(log.absoluteFilePath());
    ui->pbProgress->setValue(ui->pbProgress->value() + 1);
  }

  ui->pbProgress->setVisible(false);
  ui->teLog->append("End of converting.");
  QApplication::restoreOverrideCursor();
}

void nTrain::ConvertLog(QString filePath) {
  QFile filelog(filePath);
  if (!filelog.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QString line;
  QTextStream in(&filelog);

  QString bpnName = filePath.mid(0, filePath.lastIndexOf(".log")) + ".bpnl";
  QFile filebpn(bpnName);

  if (!filebpn.open(QIODevice::WriteOnly))
    return;
  QTextStream out(&filebpn);

  while(!in.atEnd()) {
    line = in.readLine();
    FENtoInput(line, out);
    out<<"\n";
  }

  filebpn.close();
  filelog.close();
}

void nTrain::FENtoInput(QString line, QTextStream &out) {
  int i=0;
  int j=0;

  while(i<256 && j<line.length()) {	//	this is for 64 squares
    switch(line[j].toLatin1()) {
    case 'P':	//	white pawn - code 0001
      out<<"0001";
      //out<<false<<false<<false<<true;
      i +=4;
      break;
    case 'p':	//	black pawn - code 1001
      out<<"1001";
      //out<<true<<false<<false<<true;
      i +=4;
      break;
    case 'N':	//	white kNight - code 0010
      out<<"0010";
      //out<<false<<false<<true<<false;
      i +=4;
      break;
    case 'n':	//	black kNight - code 1010
      out<<"1010";
      //out<<true<<false<<true<<false;
      i +=4;
      break;
    case 'B':	//	white bishop - code 0011
      out<<"0011";
      //out<<false<<false<<true<<true;
      i +=4;
      break;
    case 'b':	//	black bishop - code 1011
      out<<"1011";
      //out<<true<<false<<true<<true;
      i +=4;
      break;
    case 'R':	//	white rook - code 0100
      out<<"0100";
      //out<<false<<true<<false<<false;
      i +=4;
      break;
    case 'r':	//	black rook - code 1100
      out<<"1100";
      //out<<true<<true<<false<<false;
      i +=4;
      break;
    case 'Q':	//	white queen - code 0101
      out<<"0101";
      //out<<false<<true<<false<<true;
      i +=4;
      break;
    case 'q':	//	black queen - code 1101
      out<<"1101";
      //out<<true<<true<<false<<true;
      i +=4;
      break;
    case 'K':	//	white king - code 0110
      out<<"0110";
      //out<<false<<true<<true<<false;
      i +=4;
      break;
    case 'k':	//	black king - code 1110
      out<<"1110";
      //out<<true<<true<<true<<false;
      i +=4;
      break;
    }

    if(line[j] <= '8' && line[j] >= '1') {
      int emptySquares = line[j].toAscii() - '0';
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
    switch(line[j].toLatin1()) {
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

  int braket = line.lastIndexOf('[');
  QString eval = line.mid(braket + 1, line.lastIndexOf(']') - braket - 1);
  out<<" "<<eval;	//	get evaluation for current position
}

void nTrain::on_pbTest_clicked() {
  QDir logDir(ui->leTrainPath->text(), "*.bpnl", QDir::Name, QDir::Files);

  if(!logDir.exists()) return;

  QCursor wait(Qt::WaitCursor);
  QApplication::setOverrideCursor(wait);

  ui->pbProgress->reset();

  ui->teLog->append("Testing:");
  //ui->teLog->scroll(0,50);
  ui->teLog->repaint();

  double total_error = 0, current_error;
  int total_count=0, i=0;

  if(logDir.entryInfoList().count() >0) {
    ui->pbProgress->setRange(0, logDir.entryInfoList().count());
    ui->pbProgress->setValue(0);
    ui->pbProgress->setVisible(true);

    foreach(QFileInfo log, logDir.entryInfoList()) {
      i = 0;
      current_error = TestOverFile(log.absoluteFilePath(), i);

      if(i > 0) {
	total_error += current_error;
	total_count += i;

	current_error /= (float)i;
      }

      ui->teLog->append("Mean error over " + log.baseName() + ": " + QString::number(current_error));
      //ui->teLog->scroll(0,50);
      ui->teLog->repaint();

      ui->pbProgress->setValue(ui->pbProgress->value() + 1);
    }
  }
  else {
    QStringList logFilter("*.log");
    logDir.setNameFilters(logFilter);

    ui->pbProgress->setRange(0, logDir.entryInfoList().count());
    ui->pbProgress->setValue(0);
    ui->pbProgress->setVisible(true);

    foreach(QFileInfo log, logDir.entryInfoList()) {
      i = 0;
      current_error = TestOverRawFile(log.absoluteFilePath(), i);

      if(i > 0) {
	total_error += current_error;
	total_count += i;

	current_error /= (float)i;
      }

      ui->teLog->append("Mean error over " + log.baseName() + ": " + QString::number(current_error));
      //ui->teLog->scroll(0,50);
      ui->teLog->repaint();

      ui->pbProgress->setValue(ui->pbProgress->value() + 1);
    }
  }

  ui->pbProgress->setVisible(false);

  if(total_count > 0)
    total_error /= (float)total_count;

  if(ui->pbProgress->value() >1)
    ui->teLog->append("Total mean error: " + QString::number(total_error));


  QApplication::restoreOverrideCursor();
}

double nTrain::TestOverFile(QString filePath, int &i) {
  QFile filelog(filePath);
  if (!filelog.open(QIODevice::ReadOnly | QIODevice::Text))
    return false;

  QTextStream in(&filelog);

  double input[262], output[1], err, sumerror=0;

  while(!in.atEnd()) {
    char* line = in.readLine().toLatin1().data();

    for(int j=0; j<262; ++j) {
      input[j] = (double)((int)line[j] - 48);
    }

    output[0] = atof((char*)(line + 263));

    bpn0->Run(input);

    //        ui->teLog->append(QVariant::fromValue(bpn0->train_output[0]).toString() + " : " + QVariant::fromValue(bpn0->layers[bpn0->size - 1]->products[0]).toString());
    //        ui->teLog->repaint();

    err = output[0] - (bpn0->layers[bpn0->size-1]->products[0]);
    if(err < 0) err = -err;
    sumerror += err;

    ++i;
  }

  filelog.close();

  if(i ==0) return 0;

  return sumerror;
}

double nTrain::TestOverRawFile(QString filePath, int &i) {
  QFile filelog(filePath);
  if (!filelog.open(QIODevice::ReadOnly | QIODevice::Text))
    return false;

  QTextStream in(&filelog);

  double sumerror=0, err;

  while(!in.atEnd()) {
    char* line = in.readLine().toLatin1().data();

    bpn0->Run(line);

    err = bpn0->train_output[0] - bpn0->layers[bpn0->size - 1]->products[0];

    //        ui->teLog->append(QVariant::fromValue(bpn0->train_output[0]).toString() + " : " + QVariant::fromValue(bpn0->layers[bpn0->size - 1]->products[0]).toString());
    //        ui->teLog->repaint();

    if(err < 0) err = -err;
    sumerror += err;

    ++i;
  }

  filelog.close();

  if(i ==0) return 0;

  return sumerror;
}

void nTrain::on_pbStop_clicked() {

}
