#include "Food.h"

#include <ostream>

namespace calorix {

    Food::Food()
        : foodId(0), name(""), caloriesPer100g(0), proteinPer100g(0), carbsPer100g(0), fatPer100g(0) {
    }

    Food::Food(unsigned foodId,
        const std::string& name,
        double caloriesPer100g,
        double proteinPer100g,
        double carbsPer100g,
        double fatPer100g)
        : foodId(foodId),
        name(name),
        caloriesPer100g(caloriesPer100g),
        proteinPer100g(proteinPer100g),
        carbsPer100g(carbsPer100g),
        fatPer100g(fatPer100g) {
    }

    unsigned Food::getFoodId() const {
        return foodId;
    }

    const std::string& Food::getName() const {
        return name;
    }

    double Food::getCaloriesPer100g() const {
        return caloriesPer100g;
    }

    double Food::getProteinPer100g() const {
        return proteinPer100g;
    }

    double Food::getCarbsPer100g() const {
        return carbsPer100g;
    }

    double Food::getFatPer100g() const {
        return fatPer100g;
    }

    void Food::setCaloriesPer100g(double calories) {
        if (calories >= 0) {
            caloriesPer100g = calories;
        }
    }

    double Food::calculateCalories(double grams) const {
        return caloriesPer100g * grams / 100.0;
    }

    double Food::calculateProtein(double grams) const {
        return proteinPer100g * grams / 100.0;
    }

    double Food::calculateCarbs(double grams) const {
        return carbsPer100g * grams / 100.0;
    }

    double Food::calculateFat(double grams) const {
        return fatPer100g * grams / 100.0;
    }

    void Food::print(std::ostream& os) const {
        os << name
            << " | " << caloriesPer100g << " kcal/100g"
            << " | protein: " << proteinPer100g
            << " | carbs: " << carbsPer100g
            << " | fat: " << fatPer100g << '\n';
    }

}