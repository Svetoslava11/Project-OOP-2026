#include "Exercise.h"
#include <ostream>

namespace calorix {

    Exercise::Exercise()
        : exerciseId(0), name(""), caloriesBurnedPerHour(0), muscleGroup(MuscleGroup::CARDIO), durationMinutes(30) {
    }

    Exercise::Exercise(unsigned exerciseId,
        const std::string& name,
        double caloriesBurnedPerHour,
        MuscleGroup muscleGroup,
        unsigned durationMinutes)
        : exerciseId(exerciseId),
        name(name),
        caloriesBurnedPerHour(caloriesBurnedPerHour),
        muscleGroup(muscleGroup),
        durationMinutes(durationMinutes == 0 ? 30 : durationMinutes) {
    }

    unsigned Exercise::getExerciseId() const {
        return exerciseId;
    }

    const std::string& Exercise::getName() const {
        return name;
    }

    double Exercise::getCaloriesBurnedPerHour() const {
        return caloriesBurnedPerHour;
    }

    MuscleGroup Exercise::getMuscleGroup() const {
        return muscleGroup;
    }

    unsigned Exercise::getDurationMinutes() const {
        return durationMinutes;
    }

    double Exercise::calculateBurnedCalories(unsigned durationMinutes) const {
        return caloriesBurnedPerHour * durationMinutes / 60.0;
    }

    double Exercise::getKnapsackValue() const {
        return calculateBurnedCalories(durationMinutes);
    }

    void Exercise::print(std::ostream& os) const {
        os << name
            << " | " << caloriesBurnedPerHour << " kcal/hour"
            << " | group: " << toString(muscleGroup)
            << " | duration: " << durationMinutes << " min\n";
    }

}