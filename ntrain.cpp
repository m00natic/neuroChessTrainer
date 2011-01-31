#include <QFileDialog>
#include <QTextStream>
#include <ctime>
#include "ntrain.h"
#include "ui_ntrain.h"
#include "bpn.h"

nTrain::nTrain(QWidget *parent)
  : QDialog(parent), ui(new Ui::nTrainClass) {
  ui->setupUi(this);
  ui->pbProgress->setVisible(false);
  ui->teLog->ensureCursorVisible();

  bpn0 = new BPN();
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

  bpn0->ChangeThreads(ui->sbThreads->value() -1);

  if(logDir.entryInfoList().count() >0) {
    ui->pbProgress->setRange(0, logDir.entryInfoList().count()*ui->sbEpochs->value());
    ui->pbProgress->setValue(0);
    ui->pbProgress->setVisible(true);

    foreach(QFileInfo log, logDir.entryInfoList()) {
      ui->teLog->append("Training over " + log.baseName());
      //ui->teLog->scroll(0,50);
      ui->teLog->repaint();

      for(int i=0; i < ui->sbEpochs->value(); ++i) {
	if(bpn0->TrainOverFile(log.absoluteFilePath().toLocal8Bit().constData())) {
	  bpn0->SaveToFile((char*)"./bpn~");
	}
	else {
	  delete bpn0;
	  bpn0 = new BPN((char*)"./bpn~", ui->sbThreads->value() -1);

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
	if(bpn0->TrainOverRawFile(log.absoluteFilePath().toLocal8Bit().constData())) {
	  bpn0->SaveToFile((char*)"./bpn~");
	}
	else {
	  delete bpn0;
	  bpn0 = new BPN((char*)"./bpn~", ui->sbThreads->value() -1);

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

void nTrain::on_pbBrowseDir_clicked() {
  QString folder = QFileDialog::getExistingDirectory(this, tr("Locate Train Folder"), ".");
  if(!folder.isEmpty())
    ui->leTrainPath->setText(folder);
}

void nTrain::on_pbBrowseFile_clicked() {
  QString file = QFileDialog::getOpenFileName(this, tr("Choose bpn file"), ui->leOpenPath->text());
  if(!file.isEmpty()) {
    ui->leOpenPath->setText(file);

    delete bpn0;

    QCursor wait(Qt::WaitCursor);
    QApplication::setOverrideCursor(wait);

    try {
      bpn0 = new BPN(file.toLatin1().data(), ui->sbThreads->value() -1);
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
  int i=0, j=0;

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

  time_t rawtime;

  ui->teLog->append("Testing:");
  time ( &rawtime );
  ui->teLog->append(ctime(&rawtime));
  //ui->teLog->scroll(0,50);
  ui->teLog->repaint();

  double total_error = 0, current_error;
  int total_count=0, i;

  bpn0->ChangeThreads(ui->sbThreads->value() -1);

  if(logDir.entryInfoList().count() >0) {
    ui->pbProgress->setRange(0, logDir.entryInfoList().count());
    ui->pbProgress->setValue(0);
    ui->pbProgress->setVisible(true);

    foreach(QFileInfo log, logDir.entryInfoList()) {
      i = 0;
      current_error = bpn0->TestOverFile(log.absoluteFilePath().toLocal8Bit().constData(), i);

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
      current_error = bpn0->TestOverRawFile(log.absoluteFilePath().toLocal8Bit().constData(), i);

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

  time ( &rawtime );
  ui->teLog->append(ctime(&rawtime));

  QApplication::restoreOverrideCursor();
}

void nTrain::on_pbStop_clicked() {

}
