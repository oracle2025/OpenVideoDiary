#include <QApplication>
#include <QMainWindow>
#include <QSplitter>
#include <QPushButton>
#include <QCalendarWidget>
#include <QCameraViewfinder>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraImageCapture>
#include <QMediaRecorder>
#include <QUrl>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QStandardPaths> //QStandardPaths::AppLocalDataLocation For Video Files
#include <QUuid>
#include <QDateTime>
#include <QDir>
#include <QStackedWidget>
#include <QTableView>
#include <QTextCharFormat>
// Filename: 20170101_UUID.mov
// video_diary_data.sqlite
#include "rvboxlayout.h"

#include <iostream>

void print_codecs(QMediaRecorder *recorder)
{
	for (auto &i :  recorder->supportedAudioCodecs()) {
		std::cout << "Audio Codec: " << i.toStdString() << std::endl;
	}
	for (auto &i :  recorder->supportedContainers()) {
		std::cout << "Container: " << i.toStdString() << std::endl;
	}
	for (auto &i :  recorder->supportedVideoCodecs()) {
		std::cout << "Video Codec: " << i.toStdString() << std::endl;
	}
}


QString uuid() {
	auto result = QUuid::createUuid().toString();
	result.remove(0,1);
	result.remove(result.size()-1,1);
	return result;
}

QString date_str() {
	return QDateTime::currentDateTime().toString("yyyyMMdd");
}
QVariant addEntry(QSqlQuery &q, const QString &title, const QString &filename, const QDate &created_on);
QVariant addEntry(const QString &title, const QString &filename, const QDate &created_on)
{
	std::cout << "Title: " << title.toStdString() << std::endl;
	std::cout << "Filename: " << filename.toStdString() << std::endl;
	std::cout << "Date: " << created_on.toString().toStdString() << std::endl;
	QSqlQuery q;
	if (!q.prepare(QLatin1String("insert into entries(title, filename, created_on) values(?, ?, ?)"))) {
		std::cerr << q.lastError().text().toStdString() << std::endl;
		return QVariant();//q.lastError();
	}
	return addEntry(q, title, filename, created_on);
}

QVariant addEntry(QSqlQuery &q, const QString &title, const QString &filename, const QDate &created_on)
{
	q.addBindValue(title);
	q.addBindValue(filename);
	q.addBindValue(created_on);
	q.exec();
	return q.lastInsertId();
}

QSqlError initDb()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
	                    + QDir::separator()
	                                        + "diary.sqlite");

	if (!db.open())
	    return db.lastError();

	QStringList tables = db.tables();
	if (tables.contains("entries", Qt::CaseInsensitive))
	    return QSqlError();

	QSqlQuery q;
	if (!q.exec(QLatin1String("create table entries(id integer primary key, title varchar, filename varchar, created_on date)")))
	    return q.lastError();

	if (!q.prepare(QLatin1String("insert into entries(title, filename, created_on) values(?, ?, ?)")))
		return q.lastError();

	return QSqlError("", "", QSqlError::NoError, "");

}

QSqlRelationalTableModel *create_model(QObject *parent)
{
	auto model = new QSqlRelationalTableModel(parent);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("entries");

    // Remember the indexes of the columns
    //authorIdx = model->fieldIndex("author");
    //genreIdx = model->fieldIndex("genre");

    // Set the relations to the other database tables
    //model->setRelation(authorIdx, QSqlRelation("authors", "id", "name"));
    //model->setRelation(genreIdx, QSqlRelation("genres", "id", "name"));

    // Set the localized header captions
    //model->setHeaderData(authorIdx, Qt::Horizontal, tr("Author Name"));
    //model->setHeaderData(genreIdx, Qt::Horizontal, tr("Genre"));
    model->setHeaderData(model->fieldIndex("title"), Qt::Horizontal, "Title");
    model->setHeaderData(model->fieldIndex("filename"), Qt::Horizontal, "Filename");
    model->setHeaderData(model->fieldIndex("created_on"), Qt::Horizontal, "Date");

	return model;
}

int main(int argc, char *argv[])
{


	QApplication a(argc, argv);
	QCoreApplication::setApplicationName("OpenVideoDiary");

	auto data_location = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

	auto database_filename = data_location + ("/diary.sqlite");

	if (!QSqlDatabase::drivers().contains("QSQLITE")) {
	    std::cout << "DB SQLITE NOT AVAILABLE" << std::endl;
	    return -1;
	}

	auto err = initDb();
	if (err.type() != QSqlError::NoError) {
		std::cout << "DB Error: " << err.text().toStdString() << std::endl;
		//showError(err);
		return -1;
	}


	std::cout << "Data Path: " << QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation).toStdString() << std::endl;
	std::cout << "UUid: " << uuid().toStdString() << std::endl;
	std::cout << "Date: " << QDateTime::currentDateTime().toString("yyyyMMdd").toStdString() << std::endl;

	QMainWindow w;

	auto stack = new QStackedWidget;

	auto s = new QSplitter;
	stack->addWidget(s);
	w.setCentralWidget(stack);

	auto r = new QPushButton("Record");
	r->setFlat(true);
	r->setStyleSheet("Text-align:left");
	auto c = new QCalendarWidget;
	auto cc = new QWidget;


	auto tab = new QTableView;
	auto model = create_model(tab);
	if (!model->select()) {
	 	std::cerr << model->lastError().text().toStdString() << std::endl;
	}
	tab->setModel(model);
	tab->setColumnHidden(model->fieldIndex("id"), true);
	tab->setSelectionMode(QAbstractItemView::SingleSelection);
	model->setFilter(QString("created_on='%1'").arg(QDate::currentDate().toString(Qt::ISODate)));

	QObject::connect(c, &QCalendarWidget::clicked, [&] (const QDate &date) {
			model->setFilter(QString("created_on='%1'").arg(date.toString(Qt::ISODate)));
			});

	QObject::connect(c, &QCalendarWidget::currentPageChanged, [&] (int year, int month) {
			
			if (QDate::currentDate().year() == year && QDate::currentDate().month() == month) {
				auto f = c->dateTextFormat(QDate::currentDate());
				f.setBackground(Qt::red);
				f.setForeground(Qt::white);
				c->setDateTextFormat(QDate::currentDate(), f);
			}

			QDate d(year, month, 1);
			auto d2 = d.addMonths(1).addDays(-1);
			std::cout << d.toString(Qt::ISODate).toStdString() << std::endl;
			std::cout << d2.toString(Qt::ISODate).toStdString() << std::endl;
			QSqlQuery q;
			if (!q.prepare(QLatin1String("SELECT DISTINCT created_on FROM entries WHERE created_on BETWEEN ? AND ?"))) {
			    return;
			}
			q.addBindValue(d);
			q.addBindValue(d2);
			q.exec();
			while (q.next()) {
				auto d = q.value(0).toDate();
				std::cout << "calDate: " << d.toString(Qt::ISODate).toStdString() << std::endl;
				auto f = c->dateTextFormat(d);
				//f.setFontUnderline(true);
				f.setFontWeight(QFont::Bold);
				c->setDateTextFormat(d, f);
			}
			//SELECT * FROM entires WHERE created_on >= year-month-01 AND created_on <= year-month-31
			//BETWEEN date('1004-01-01') AND date('1980-12-31')
			});

	cc->setLayout(
			RVBoxLayout::create()
			-> addWidget(c)
			//-> addStretch(1)
			-> addWidget(tab)
			-> addWidget(r)
			-> setStretch(1, 1) );
	s->addWidget(cc);

	auto v = new QCameraViewfinder;
	auto vv = new QWidget;
	vv->setLayout(
			RVBoxLayout::create()
			-> addWidget(v)
			-> addStretch(1) );
	s->addWidget(vv);

	s->setStretchFactor(0, 0);
	s->setStretchFactor(1, 1);

	auto rec = new QWidget;
	stack->addWidget(rec);

	auto camera = new QCamera;
	camera->setViewfinder(v);

	//auto imageCapture = new QCameraImageCapture(camera);

	//camera->setCaptureMode(QCamera::CaptureStillImage);
	camera->setCaptureMode(QCamera::CaptureVideo);
	camera->start();

	auto recorder = new QMediaRecorder(camera);
	QObject::connect(recorder, static_cast<void(QMediaRecorder::*)(QMediaRecorder::Error)>(&QMediaRecorder::error),
			    [&](QMediaRecorder::Error error){
			    std::cout << recorder->errorString().toStdString() << std::endl;
			    
			    });

	/*
	QAudioEncoderSettings audioSettings;
	audioSettings.setCodec("audio/mpeg");
	audioSettings.setQuality(QMultimedia::HighQuality);
	recorder->setAudioSettings(audioSettings);
	QVideoEncoderSettings videoSettings;
	videoSettings.setCodec("video/mpeg");
	recorder->setVideoSettings(videoSettings);
*/

	auto video_dir = data_location + QDir::separator() + "videos" + QDir::separator();
	if (!QDir(video_dir).exists()) {
		QDir().mkpath(video_dir);
	}

	auto filename = QUrl::fromLocalFile(video_dir + date_str() + "-" + uuid());
	std::cout << "Filename: " << filename.toLocalFile().toStdString() << std::endl;
	recorder->setOutputLocation(filename);

	QObject::connect(r, &QPushButton::clicked, [&] () {
			if (recorder->state() == QMediaRecorder::RecordingState) {
				std::cout << "Stopping" << std::endl;
				recorder->stop();

				addEntry("New Entry", recorder->outputLocation().fileName(), QDate::currentDate());

				auto filename = QUrl::fromLocalFile(video_dir + date_str() + "-" + uuid());
				std::cout << "Filename: " << filename.toLocalFile().toStdString() << std::endl;
				recorder->setOutputLocation(filename);
			} else {
				std::cout << "Recording" << std::endl;
				recorder->record();
			}
			});

	w.show();
	
	return a.exec();
}
