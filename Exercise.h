#pragma once
#include "Enums.h"

#include <iosfwd>
#include <string>

namespace calorix {

    class Exercise {
    private:
        unsigned exerciseId;
        std::string name;
        double caloriesBurnedPerHour;
        MuscleGroup muscleGroup;
        unsigned durationMinutes;

    public:
        Exercise();
        Exercise(unsigned exerciseId,
            const std::string& name,
            double caloriesBurnedPerHour,
            MuscleGroup muscleGroup,
            unsigned durationMinutes = 30);

        unsigned getExerciseId() const;
        const std::string& getName() const;
        double getCaloriesBurnedPerHour() const;
        MuscleGroup getMuscleGroup() const;
        unsigned getDurationMinutes() const;

        double calculateBurnedCalories(unsigned durationMinutes) const;
        double getKnapsackValue() const;

        void print(std::ostream& os) const;
    };

}