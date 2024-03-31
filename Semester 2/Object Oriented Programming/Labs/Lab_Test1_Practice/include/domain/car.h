#include <iostream>
#include <string>

class Car {
 private:
  std::string manufacturer, model, color;
  int year;

 public:
  Car(const std::string& manufacturer, const std::string& model, const int year,
      const std::string& color);
  ~Car();
  Car(const Car& car);
  bool operator==(const Car& otherCar) const;
  std::string getManufacturer() const;
  std::string getModel() const;
  int getYear() const;
  std::string getColor() const;
  void setManufacturer(const std::string& manufacturer);
  void setModel(const std::string& model);
  void setYear(const int year);
  void setColor(const std::string& color);
  std::string toString() const;
};