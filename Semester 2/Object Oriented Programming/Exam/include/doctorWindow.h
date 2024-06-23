#pragma once
#include "doctorSession.h"
#include "domain.h"
#include "observer.h"
#include <QCheckBox>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class DoctorWindow : public QWidget, public Observer {
private:
  Doctor* doctor;
  DoctorSession* session;
  QVBoxLayout* layout;
  QListWidget* patientList;
  QCheckBox* checkBox;
  QLineEdit* patientNameLineEdit;
  QLineEdit* patientDiagnosisLineEdit;
  QLineEdit* patientSpecialisationLineEdit;
  QLineEdit* patientCurrentDoctorLineEdit;
  QLineEdit* patientDateLineEdit;
  QPushButton* addPatientButton;
  QPushButton* updatePatiendbutton;

public:
  DoctorWindow(Doctor* doctor, DoctorSession* session) : doctor(doctor), session(session) {
    session->registerObserver(*this);
    setWindowTitle(QString::fromStdString(doctor->getName()));
    layout = new QVBoxLayout(this);
    patientList = new QListWidget(this);
    layout->addWidget(patientList);
    checkBox = new QCheckBox(this);
    layout->addWidget(checkBox);
    patientNameLineEdit = new QLineEdit(this);
    layout->addWidget(patientNameLineEdit);
    patientDiagnosisLineEdit = new QLineEdit(this);
    layout->addWidget(patientDiagnosisLineEdit);
    patientSpecialisationLineEdit = new QLineEdit(this);
    layout->addWidget(patientSpecialisationLineEdit);
    patientCurrentDoctorLineEdit = new QLineEdit(this);
    layout->addWidget(patientCurrentDoctorLineEdit);
    patientDateLineEdit = new QLineEdit(this);
    layout->addWidget(patientDateLineEdit);
    addPatientButton = new QPushButton("Add patient", this);
    layout->addWidget(addPatientButton);
    updatePatiendbutton = new QPushButton("Update patient", this);
    layout->addWidget(updatePatiendbutton);

    QPushButton::connect(addPatientButton, &QPushButton::clicked, this, &DoctorWindow::addPatientButtonHandler);
    QCheckBox::connect(checkBox, &QCheckBox::checkStateChanged, this, &DoctorWindow::update);
    QPushButton::connect(updatePatiendbutton, &QPushButton::clicked, this, &DoctorWindow::updatePatientButtonHandler);

    update();
    show();
  }
  ~DoctorWindow() {}

  void update() override {
    patientList->clear();
    if (!checkBox->isChecked()) {
      vector<Patient*> patients = session->getPatientsWithSpecialisationAndUndiagnosed(doctor->getSpecialisation());
      for (auto p : patients) {
        patientList->addItem(QString::fromStdString(p->toStr()));
      }
    } else {
      vector<Patient*> patients = session->getPatientWithDoctorName(doctor->getName());
      for (auto p : patients) {
        patientList->addItem(QString::fromStdString(p->toStr()));
      }
    }
  }

  void addPatientButtonHandler() {
    string name, diagnosis, specialisation, doctor, date;
    name = patientNameLineEdit->text().toStdString();
    if (name == "") {
      QMessageBox::critical(this, "Error", "Empty patient name");
      return;
    }
    diagnosis = patientDiagnosisLineEdit->text().toStdString();
    specialisation = patientSpecialisationLineEdit->text().toStdString();
    doctor = patientCurrentDoctorLineEdit->text().toStdString();
    date = patientDateLineEdit->text().toStdString();
    if (date < "21.6.2024") {
      QMessageBox::critical(this, "Error", "Date cannot be in the past");
      return;
    }
    Date d = session->createDate(date);
    Patient* p = new Patient(name, diagnosis, specialisation, doctor, d);
    if (!session->addPatient(p)) {
      QMessageBox::critical(this, "Error", "Patient not added");
      return;
    }
    session->notify();
  }

  void updatePatientButtonHandler() {
    string name = patientNameLineEdit->text().toStdString();
    string diagnosis = session->getDiagnosis(name);
    string specialisation = patientSpecialisationLineEdit->text().toStdString();
    if (diagnosis != "" && diagnosis != "undiagnosed") {
      QMessageBox::critical(this, "Error", "Cannot change diagnosis of patient");
      return;
    }
    string doc_name = session->getDoctorNameBySpecialisation(specialisation);
    if (patientDiagnosisLineEdit->text().toStdString() == "undiagnosed") {
      QMessageBox::critical(this, "Error", "Cannot undiagnose a patient");
      return;
    }
    session->updatePatient(name, patientDiagnosisLineEdit->text().toStdString(), specialisation, doc_name);
  }
};