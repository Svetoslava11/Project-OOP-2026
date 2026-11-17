#pragma once

#include "Date.h"
#include "Exercise.h"
#include "Food.h"

#include <iosfwd>

namespace calorix {

    class FoodEntry {
    private:
        unsigned entryId;
        const Food* food;
        double quantityGrams;
        Date date;

    public:
        FoodEntry();
        FoodEntry(unsigned entryId, const Food* food, double quantityGrams, const Date& date);

        unsigned getEntryId() const;
        const Food* getFood() const;
        double getQuantityGrams() const;
        Date getDate() const;

        double getCalories() const;
        double getProtein() const;
        double getCarbs() const;
        double getFat() const;

        void print(std::ostream& os) const;
    };

    class ExerciseEntry {
    private:
        unsigned entryId;
        const Exercise* exercise;
        unsigned durationMinutes;
        Date date;

    public:
        ExerciseEntry();
        ExerciseEntry(unsigned entryId, const Exercise* exercise, unsigned durationMinutes, const Date& date);

        unsigned getEntryId() const;
        const Exercise* getExercise() const;
        unsigned getDurationMinutes() const;
        Date getDate() const;

        double getBurnedCalories() const;

        void print(std::ostream& os) const;
    };

}