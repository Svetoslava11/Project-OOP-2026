#include "FitnessGoal.h"

#include <ostream>

namespace calorix {

    FitnessGoal::FitnessGoal()
        : goalType(GoalType::MAINTENANCE), targetValue(0), startDate(), deadline(), achieved(false) {
    }

    FitnessGoal::FitnessGoal(GoalType goalType, double targetValue, const Date& startDate, const Date& deadline)
        : goalType(goalType), targetValue(targetValue), startDate(startDate), deadline(deadline), achieved(false) {
    }

    GoalType FitnessGoal::getGoalType() const {
        return goalType;
    }

    double FitnessGoal::getTargetValue() const {
        return targetValue;
    }

    Date FitnessGoal::getStartDate() const {
        return startDate;
    }

    Date FitnessGoal::getDeadline() const {
        return deadline;
    }

    bool FitnessGoal::isAchieved() const {
        return achieved;
    }

    void FitnessGoal::setAchieved(bool achieved) {
        this->achieved = achieved;
    }

    void FitnessGoal::print(std::ostream& os) const {
        os << "Goal: " << toString(goalType)
            << " | target: " << targetValue
            << " | start: " << startDate.toString()
            << " | deadline: " << deadline.toString()
            << " | achieved: " << (achieved ? "yes" : "no") << '\n';
    }

}