#pragma once

#include <iosfwd>
#include <string>

namespace calorix {

    class Food {
    private:
        unsigned foodId;
        std::string name;
        double caloriesPer100g;
        double proteinPer100g;
        double carbsPer100g;
        double fatPer100g;

    public:
        Food();
        Food(unsigned foodId,
            const std::string& name,
            double caloriesPer100g,
            double proteinPer100g,
            double carbsPer100g,
            double fatPer100g);

        unsigned getFoodId() const;
        const std::string& getName() const;
        double getCaloriesPer100g() const;
        double getProteinPer100g() const;
        double getCarbsPer100g() const;
        double getFatPer100g() const;

        void setCaloriesPer100g(double calories);

        double calculateCalories(double grams) const;
        double calculateProtein(double grams) const;
        double calculateCarbs(double grams) const;
        double calculateFat(double grams) const;

        void print(std::ostream& os) const;
    };

}