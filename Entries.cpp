#include "Entries.h"
#include <ostream>

namespace calorix {

    FoodEntry::FoodEntry() : entryId(0), food(nullptr), quantityGrams(0), date() {}

    FoodEntry::FoodEntry(unsigned entryId, const Food* food, double quantityGrams, const Date& date)
        : entryId(entryId), food(food), quantityGrams(quantityGrams), date(date) {
    }

    unsigned FoodEntry::getEntryId() const {
        return entryId;
    }

    const Food* FoodEntry::getFood() const {
        return food;
    }

    double FoodEntry::getQuantityGrams() const {
        return quantityGrams;
    }

    Date FoodEntry::getDate() const {
        return date;
    }

    double FoodEntry::getCalories() const {
        return food ? food->calculateCalories(quantityGrams) : 0;
    }

    double FoodEntry::getProtein() const {
        return food ? food->calculateProtein(quantityGrams) : 0;
    }

    double FoodEntry::getCarbs() const {
        return food ? food->calculateCarbs(quantityGrams) : 0;
    }

    double FoodEntry::getFat() const {
        return food ? food->calculateFat(quantityGrams) : 0;
    }

    void FoodEntry::print(std::ostream& os) const {
        if (!food) {
            os << "Invalid food entry\n";
            return;
        }

        os << food->getName()
            << " | quantity: " << quantityGrams << "g"
            << " | calories: " << getCalories()
            << " | date: " << date.toString() << '\n';
    }

    ExerciseEntry::ExerciseEntry() : entryId(0), exercise(nullptr), durationMinutes(0), date() {}

    ExerciseEntry::ExerciseEntry(unsigned entryId, const Exercise* exercise, unsigned durationMinutes, const Date& date)
        : entryId(entryId), exercise(exercise), durationMinutes(durationMinutes), date(date) {
    }

    unsigned ExerciseEntry::getEntryId() const {
        return entryId;
    }

    const Exercise* ExerciseEntry::getExercise() const {
        return exercise;
    }

    unsigned ExerciseEntry::getDurationMinutes() const {
        return durationMinutes;
    }

    Date ExerciseEntry::getDate() const {
        return date;
    }

    double ExerciseEntry::getBurnedCalories() const {
        return exercise ? exercise->calculateBurnedCalories(durationMinutes) : 0;
    }

    void ExerciseEntry::print(std::ostream& os) const {
        if (!exercise) {
            os << "Invalid exercise entry\n";
            return;
        }

        os << exercise->getName()
            << " | duration: " << durationMinutes << " min"
            << " | burned: " << getBurnedCalories()
            << " kcal | date: " << date.toString() << '\n';
    }

}