#pragma once

#include "Date.h"
#include "Enums.h"

#include <iosfwd>

namespace calorix {

    class FitnessGoal {
    private:
        GoalType goalType;
        double targetValue;
        Date startDate;
        Date deadline;
        bool achieved;

    public:
        FitnessGoal();
        FitnessGoal(GoalType goalType, double targetValue, const Date& startDate, const Date& deadline);

        GoalType getGoalType() const;
        double getTargetValue() const;
        Date getStartDate() const;
        Date getDeadline() const;
        bool isAchieved() const;

        void setAchieved(bool achieved);
        void print(std::ostream& os) const;
    };

}