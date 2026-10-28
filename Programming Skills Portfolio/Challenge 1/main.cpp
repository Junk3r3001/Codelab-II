#include <iostream>
#include <string>
#include <cctype>
#include <iomanip>

const double FUEL_CONSUMPTION_PER_100_MILES = 9.66;
const double DIESEL_CO2_PER_LITRE = 2.68;
const double PETROL_CO2_PER_LITRE = 2.31;

double getDistanceTravelled() {
    double distance = 0.0;
    while (true) {
        std::cout << "Enter journey distance in miles: ";
        if (std::cin >> distance && distance >= 0.0) {
            return distance;
        }
        std::cout << "Invalid input. Distance must be a non-negative number.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

char getFuelType() {
    char fuelChoice;
    while (true) {
        std::cout << "Enter fuel type - [D]iesel or [P]etrol: ";
        std::cin >> fuelChoice;
        fuelChoice = std::toupper(fuelChoice);

        if (fuelChoice == 'D' || fuelChoice == 'P') {
            return fuelChoice;
        }
        std::cout << "Invalid selection. Please enter 'D' for Diesel or 'P' for Petrol.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

double calculateEmissions(double distance, char fuelType) {
    double totalLitres = (distance / 100.0) * FUEL_CONSUMPTION_PER_100_MILES;
    double emissionFactor = (fuelType == 'D') ? DIESEL_CO2_PER_LITRE : PETROL_CO2_PER_LITRE;
    return totalLitres * emissionFactor;
}

int main() {
    double distance = getDistanceTravelled();
    char fuelType = getFuelType();
    double totalEmissions = calculateEmissions(distance, fuelType);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nDistance: " << distance << " miles\n";
    std::cout << "Fuel Type: " << (fuelType == 'D' ? "Diesel" : "Petrol") << "\n";
    std::cout << "Total CO2: " << totalEmissions << " kg CO2\n";

    return 0;
}