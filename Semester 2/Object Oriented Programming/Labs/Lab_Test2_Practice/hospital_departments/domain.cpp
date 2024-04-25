#include "domain.h"

bool Surgery::isEfficient() {
  return double(double(numberOfPatients) / double(numberOfDoctors)) >= 2.0;
}

string Surgery::toString() {
  string efficient;
  if (isEfficient()) {
    efficient = "true";
  } else {
    efficient = "false";
  }
  return "Surgery, " + HospitalDepartment::toString() + ", Number of patients: " + to_string(numberOfPatients) + ", Efficient: " + efficient;
}

bool NeonatalUnit::isEfficient() {
  return (averageGrade >= 8.5) && (numberOfNewborns >= numberOfMothers);
}

string NeonatalUnit::toString() {
  string efficient;
  if (isEfficient()) {
    efficient = "true";
  } else {
    efficient = "false";
  }
  return "Neonatal Unit, " + HospitalDepartment::toString() + ", Number of babies: " + to_string(numberOfNewborns) + ", Average grade: " + to_string(averageGrade) + ", Efficient: " + efficient;
}