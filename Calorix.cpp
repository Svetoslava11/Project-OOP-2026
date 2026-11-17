#include "Calorix.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace calorix {

    namespace {

        std::vector<std::string> splitLine(const std::string& line) {
            std::istringstream iss(line);
            std::vector<std::string> tokens;
            std::string token;

            while (iss >> token) {
                tokens.push_back(token);
            }

            return tokens;
        }

        bool toUnsigned(const std::string& text, unsigned& out) {
            try {
                size_t pos = 0;
                const unsigned long value = std::stoul(text, &pos);
                if (pos != text.size()) {
                    return false;
                }
                out = static_cast<unsigned>(value);
                return true;
            }
            catch (...) {
                return false;
            }
        }

        bool toDouble(const std::string& text, double& out) {
            try {
                size_t pos = 0;
                out = std::stod(text, &pos);
                return pos == text.size();
            }
            catch (...) {
                return false;
            }
        }

    }
    Calorix::Calorix(const std::string& dataFileName)
        : users(),
        foods(),
        exercises(),
        currentUser(nullptr),
        nextUserId(1),
        nextFoodId(1),
        nextExerciseId(1),
        nextEntryId(1),
        dataFileName(dataFileName) {
    }

    User* Calorix::findUserByUsername(const std::string& username) {
        for (const auto& user : users) {
            if (user->getUsername() == username) {
                return user.get();
            }
        }
        return nullptr;
    }

    const User* Calorix::findUserByUsername(const std::string& username) const {
        for (const auto& user : users) {
            if (user->getUsername() == username) {
                return user.get();
            }
        }
        return nullptr;
    }

    Food* Calorix::findFoodByName(const std::string& name) {
        for (auto& food : foods) {
            if (food->getName() == name) {
                return food.get();
            }
        }
        return nullptr;
    }

    const Food* Calorix::findFoodByName(const std::string& name) const {
        for (const auto& food : foods) {
            if (food->getName() == name) {
                return food.get();
            }
        }
        return nullptr;
    }

    Food* Calorix::findFoodById(unsigned foodId) {
        for (auto& food : foods) {
            if (food->getFoodId() == foodId) {
                return food.get();
            }
        }
        return nullptr;
    }

    const Food* Calorix::findFoodById(unsigned foodId) const {
        for (const auto& food : foods) {
            if (food->getFoodId() == foodId) {
                return food.get();
            }
        }
        return nullptr;
    }

    Exercise* Calorix::findExerciseByName(const std::string& name) {
        for (auto& exercise : exercises) {
            if (exercise->getName() == name) {
                return exercise.get();
            }
        }
        return nullptr;
    }

    const Exercise* Calorix::findExerciseByName(const std::string& name) const {
        for (const auto& exercise : exercises) {
            if (exercise->getName() == name) {
                return exercise.get();
            }
        }
        return nullptr;
    }

    Exercise* Calorix::findExerciseById(unsigned exerciseId) {
        for (auto& exercise : exercises) {
            if (exercise->getExerciseId() == exerciseId) {
                return exercise.get();
            }
        }
        return nullptr;
    }

    const Exercise* Calorix::findExerciseById(unsigned exerciseId) const {
        for (const auto& exercise : exercises) {
            if (exercise->getExerciseId() == exerciseId) {
                return exercise.get();
            }
        }
        return nullptr;
    }

    Trainee* Calorix::getCurrentTrainee() {
        if (!currentUser) {
            std::cout << "You are not logged in.\n";
            return nullptr;
        }

        Trainee* trainee = dynamic_cast<Trainee*>(currentUser);
        if (!trainee) {
            std::cout << "This command is only for trainees.\n";
            return nullptr;
        }

        return trainee;
    }

    const Trainee* Calorix::getCurrentTrainee() const {
        if (!currentUser) {
            std::cout << "You are not logged in.\n";
            return nullptr;
        }

        const Trainee* trainee = dynamic_cast<const Trainee*>(currentUser);
        if (!trainee) {
            std::cout << "This command is only for trainees.\n";
            return nullptr;
        }

        return trainee;
    }

    Admin* Calorix::getCurrentAdmin() {
        if (!currentUser) {
            std::cout << "You are not logged in.\n";
            return nullptr;
        }

        Admin* admin = dynamic_cast<Admin*>(currentUser);
        if (!admin) {
            std::cout << "This command is only for admins.\n";
            return nullptr;
        }

        return admin;
    }

    bool Calorix::isUsernameTaken(const std::string& username) const {
        return findUserByUsername(username) != nullptr;
    }

    void Calorix::createDefaultAdmin() {
        if (findUserByUsername("admin")) {
            return;
        }

        UserProfile profile(0, 0, 0, Gender::FEMALE, ActivityLevel::SEDENTARY);
        users.push_back(std::make_unique<Admin>(nextUserId++, "admin", "admin", profile));
    }

    void Calorix::run() {
        loadFromFile();

        std::cout << "Calorix started. Type help for commands. Type end to save and exit.\n";

        std::string line;
        while (true) {
            std::cout << "> ";
            if (!std::getline(std::cin, line)) {
                saveToFile();
                break;
            }

            if (line.empty()) {
                continue;
            }

            const std::vector<std::string> tokens = splitLine(line);
            if (!tokens.empty() && tokens[0] == "end") {
                saveToFile();
                std::cout << "Data saved. Goodbye.\n";
                break;
            }

            executeCommand(line);
        }
    }

    void Calorix::executeCommand(const std::string& line) {
        const std::vector<std::string> tokens = splitLine(line);
        if (tokens.empty()) {
            return;
        }

        const std::string& command = tokens[0];

        if (command == "help") {
            help();
        }
        else if (command == "register") {
            if (tokens.size() != 7 && tokens.size() != 8) {
                std::cout << "Usage: register <username> <password> <age> <weight> <height> <MALE|FEMALE> <SEDENTARY|LIGHT|MODERATE|ACTIVE|VERY_ACTIVE>\n";
                return;
            }

            unsigned age = 0;
            double weight = 0;
            double height = 0;
            Gender gender = Gender::FEMALE;
            ActivityLevel activityLevel = ActivityLevel::MODERATE;

            if (!toUnsigned(tokens[3], age) || !toDouble(tokens[4], weight) || !toDouble(tokens[5], height) || !parseGender(tokens[6], gender)) {
                std::cout << "Invalid register arguments.\n";
                return;
            }

            if (tokens.size() == 8 && !parseActivityLevel(tokens[7], activityLevel)) {
                std::cout << "Invalid activity level.\n";
                return;
            }

            registerUser(tokens[1], tokens[2], age, weight, height, gender, activityLevel);
        }
        else if (command == "login") {
            if (tokens.size() != 3) {
                std::cout << "Usage: login <username> <password>\n";
                return;
            }
            login(tokens[1], tokens[2]);
        }
        else if (command == "logout") {
            logout();
        }
        else if (command == "add-food") {
            if (tokens.size() != 6) {
                std::cout << "Usage: add-food <name> <calories/100g> <protein/100g> <carbs/100g> <fat/100g>\n";
                return;
            }

            double calories = 0;
            double protein = 0;
            double carbs = 0;
            double fat = 0;

            if (!toDouble(tokens[2], calories) || !toDouble(tokens[3], protein) || !toDouble(tokens[4], carbs) || !toDouble(tokens[5], fat)) {
                std::cout << "Invalid food arguments.\n";
                return;
            }

            addFood(tokens[1], calories, protein, carbs, fat);
        }
        else if (command == "add-exercise") {
            if (tokens.size() != 4 && tokens.size() != 5) {
                std::cout << "Usage: add-exercise <name> <calories/hour> <ARMS|CHEST|LEGS|CARDIO|BACK|SHOULDERS|CORE> [duration-minutes]\n";
                return;
            }

            double calories = 0;
            MuscleGroup group = MuscleGroup::CARDIO;
            unsigned duration = 30;

            if (!toDouble(tokens[2], calories) || !parseMuscleGroup(tokens[3], group)) {
                std::cout << "Invalid exercise arguments.\n";
                return;
            }

            if (tokens.size() == 5 && !toUnsigned(tokens[4], duration)) {
                std::cout << "Invalid duration.\n";
                return;
            }

            addExercise(tokens[1], calories, group, duration);
        }
        else if (command == "update-food") {
            if (tokens.size() != 3) {
                std::cout << "Usage: update-food <food-name> <new-calories/100g>\n";
                return;
            }

            double calories = 0;
            if (!toDouble(tokens[2], calories)) {
                std::cout << "Invalid calories.\n";
                return;
            }

            updateFood(tokens[1], calories);
        }
        else if (command == "block-user") {
            if (tokens.size() != 2) {
                std::cout << "Usage: block-user <username>\n";
                return;
            }
            blockUser(tokens[1]);
        }
        else if (command == "log-food") {
            if (tokens.size() != 3) {
                std::cout << "Usage: log-food <food-name> <quantity-grams>\n";
                return;
            }

            double grams = 0;
            if (!toDouble(tokens[2], grams)) {
                std::cout << "Invalid quantity.\n";
                return;
            }

            logFood(tokens[1], grams);
        }
        else if (command == "log-exercise") {
            if (tokens.size() != 3) {
                std::cout << "Usage: log-exercise <exercise-name> <duration-minutes>\n";
                return;
            }

            unsigned duration = 0;
            if (!toUnsigned(tokens[2], duration)) {
                std::cout << "Invalid duration.\n";
                return;
            }

            logExercise(tokens[1], duration);
        }
        else if (command == "set-goals") {
            if (tokens.size() != 4) {
                std::cout << "Usage: set-goals <MAINTENANCE|BULKING|WEIGHT_LOSS> <target-value(KG)> <deadline dd.mm.yyyy>\n";
                return;
            }

            GoalType goalType = GoalType::MAINTENANCE;
            double targetValue = 0;
            Date deadline;

            if (!parseGoalType(tokens[1], goalType) || !toDouble(tokens[2], targetValue) || !Date::parse(tokens[3], deadline)) {
                std::cout << "Invalid goal arguments.\n";
                return;
            }

            setGoals(goalType, targetValue, deadline);
        }
        else if (command == "view-daily-summary") {
            if (tokens.size() == 1) {
                viewDailySummary(Date::today());
            }
            else if (tokens.size() == 2) {
                Date date;
                if (!Date::parse(tokens[1], date)) {
                    std::cout << "Invalid date. Use dd.mm.yyyy.\n";
                    return;
                }
                viewDailySummary(date);
            }
            else {
                std::cout << "Usage: view-daily-summary [date dd.mm.yyyy]\n";
            }
        }
        else if (command == "view-progress") {
            viewProgress();
        }
        else if (command == "calculate-bmi") {
            calculateBMI();
        }
        else if (command == "calculate-bmr") {
            calculateBMR();
        }
        else if (command == "generate-workout-plan") {
            if (tokens.size() != 2) {
                std::cout << "Usage: generate-workout-plan <duration-minutes>\n";
                return;
            }

            unsigned duration = 0;
            if (!toUnsigned(tokens[1], duration)) {
                std::cout << "Invalid duration.\n";
                return;
            }

            generateWorkoutPlan(duration);
        }
        else if (command == "add-to-favorites") {
            if (tokens.size() != 2) {
                std::cout << "Usage: add-to-favorites <exercise-name>\n";
                return;
            }
            addToFavorites(tokens[1]);
        }
        else if (command == "view-favorites") {
            viewFavorites();
        }
        else if (command == "list-foods") {
            listFoods();
        }
        else if (command == "list-exercises") {
            listExercises();
        }
        else {
            std::cout << "Unknown command. Type help.\n";
        }
    }

    void Calorix::registerUser(const std::string& username,
        const std::string& password,
        unsigned age,
        double weight,
        double height,
        Gender gender,
        ActivityLevel activityLevel) {
        if (isUsernameTaken(username)) {
            std::cout << "Username is already taken.\n";
            return;
        }

        if (username.empty() || password.empty() || age == 0 || weight <= 0 || height <= 0) {
            std::cout << "Invalid user data.\n";
            return;
        }

        UserProfile profile(age, weight, height, gender, activityLevel);
        users.push_back(std::make_unique<Trainee>(nextUserId++, username, password, profile));

        std::cout << "User registered successfully.\n";
    }

    void Calorix::login(const std::string& username, const std::string& password) {
        User* user = findUserByUsername(username);

        if (!user) {
            std::cout << "User not found.\n";
            return;
        }

        if (!user->checkPassword(password)) {
            std::cout << "Wrong password.\n";
            return;
        }

        currentUser = user;
        std::cout << "Logged in as " << username << ".\n";
    }

    void Calorix::logout() {
        if (!currentUser) {
            std::cout << "No user is currently logged in.\n";
            return;
        }

        std::cout << "Logged out from " << currentUser->getUsername() << ".\n";
        currentUser = nullptr;
    }

    void Calorix::addFood(const std::string& name, double calories, double protein, double carbs, double fat) {
        if (!getCurrentAdmin()) {
            return;
        }

        if (findFoodByName(name)) {
            std::cout << "Food already exists.\n";
            return;
        }

        if (name.empty() || calories < 0 || protein < 0 || carbs < 0 || fat < 0) {
            std::cout << "Invalid food data.\n";
            return;
        }

        foods.push_back(std::make_unique<Food>(nextFoodId++, name, calories, protein, carbs, fat));
        std::cout << "Food added successfully.\n";
    }

    void Calorix::addExercise(const std::string& name,
        double caloriesBurnedPerHour,
        MuscleGroup muscleGroup,
        unsigned durationMinutes) {
        if (!getCurrentAdmin()) {
            return;
        }

        if (findExerciseByName(name)) {
            std::cout << "Exercise already exists.\n";
            return;
        }

        if (name.empty() || caloriesBurnedPerHour < 0 || durationMinutes == 0) {
            std::cout << "Invalid exercise data.\n";
            return;
        }

        exercises.push_back(std::make_unique<Exercise>(nextExerciseId++, name, caloriesBurnedPerHour, muscleGroup, durationMinutes));
        std::cout << "Exercise added successfully.\n";
    }

    void Calorix::updateFood(const std::string& foodName, double newCalories) {
        if (!getCurrentAdmin()) {
            return;
        }

        Food* food = findFoodByName(foodName);
        if (!food) {
            std::cout << "Food not found.\n";
            return;
        }

        food->setCaloriesPer100g(newCalories);
        std::cout << "Food updated successfully.\n";
    }

    void Calorix::blockUser(const std::string& username) {
        if (!getCurrentAdmin()) {
            return;
        }

        if (username == "admin") {
            std::cout << "The default admin cannot be blocked.\n";
            return;
        }

        if (currentUser && currentUser->getUsername() == username) {
            std::cout << "Cannot block the currently logged user.\n";
            return;
        }

        const auto oldSize = users.size();
        users.erase(std::remove_if(users.begin(), users.end(), [&username](const std::unique_ptr<User>& user) {
            return user->getUsername() == username;
            }),
            users.end());

        if (users.size() == oldSize) {
            std::cout << "User not found.\n";
        }
        else {
            std::cout << "User blocked and deleted.\n";
        }
    }

    void Calorix::logFood(const std::string& foodName, double quantityGrams) {
        Trainee* trainee = getCurrentTrainee();
        if (!trainee) {
            return;
        }

        const Food* food = findFoodByName(foodName);
        if (!food) {
            std::cout << "Food not found.\n";
            return;
        }

        trainee->logFood(nextEntryId++, food, quantityGrams, Date::today());
    }

    void Calorix::logExercise(const std::string& exerciseName, unsigned durationMinutes) {
        Trainee* trainee = getCurrentTrainee();
        if (!trainee) {
            return;
        }

        const Exercise* exercise = findExerciseByName(exerciseName);
        if (!exercise) {
            std::cout << "Exercise not found.\n";
            return;
        }

        trainee->logExercise(nextEntryId++, exercise, durationMinutes, Date::today());
    }

    void Calorix::setGoals(GoalType goalType, double targetValue, const Date& deadline) {
        Trainee* trainee = getCurrentTrainee();
        if (!trainee) {
            return;
        }

        FitnessGoal goal(goalType, targetValue, Date::today(), deadline);
        trainee->setGoals(goal);
        std::cout << "Goal set successfully.\n";
    }

    void Calorix::viewDailySummary(const Date& date) const {
        const Trainee* trainee = getCurrentTrainee();
        if (!trainee) {
            return;
        }

        trainee->viewDailySummary(date, std::cout);
    }

    void Calorix::viewProgress() const {
        const Trainee* trainee = getCurrentTrainee();
        if (!trainee) {
            return;
        }

        trainee->viewProgress(std::cout);
    }

    void Calorix::calculateBMI() const {
        const Trainee* trainee = getCurrentTrainee();
        if (!trainee) {
            return;
        }

        std::cout << std::fixed << std::setprecision(2) << "BMI: " << trainee->calculateBMI() << '\n';
    }

    void Calorix::calculateBMR() const {
        const Trainee* trainee = getCurrentTrainee();
        if (!trainee) {
            return;
        }

        std::cout << std::fixed << std::setprecision(2) << "BMR: " << trainee->calculateBMR() << " kcal/day\n";
    }

    void Calorix::generateWorkoutPlan(unsigned durationMinutes) const {
        const Trainee* trainee = getCurrentTrainee();
        if (!trainee) {
            return;
        }

        trainee->generateWorkoutPlan(exercises, durationMinutes, std::cout);
    }

    void Calorix::addToFavorites(const std::string& exerciseName) {
        Trainee* trainee = getCurrentTrainee();
        if (!trainee) {
            return;
        }

        const Exercise* exercise = findExerciseByName(exerciseName);
        if (!exercise) {
            std::cout << "Exercise not found.\n";
            return;
        }

        trainee->addToFavorites(exercise);
    }

    void Calorix::viewFavorites() const {
        const Trainee* trainee = getCurrentTrainee();
        if (!trainee) {
            return;
        }

        trainee->viewFavorites(std::cout);
    }

    void Calorix::listFoods() const {
        if (foods.empty()) {
            std::cout << "No foods available.\n";
            return;
        }

        std::cout << "Foods:\n";
        for (const auto& food : foods) {
            std::cout << "  - ";
            food->print(std::cout);
        }
    }

    void Calorix::listExercises() const {
        if (exercises.empty()) {
            std::cout << "No exercises available.\n";
            return;
        }

        std::cout << "Exercises:\n";
        for (const auto& exercise : exercises) {
            std::cout << "  - ";
            exercise->print(std::cout);
        }
    }

    void Calorix::help() const {
        if (!currentUser) {
            std::cout << "General commands:\n"
                << "  register <username> <password> <age> <weight> <height> <MALE|FEMALE> <SEDENTARY|LIGHT|MODERATE|ACTIVE|VERY_ACTIVE>\n"
                << "  login <username> <password>\n"
                << "  help\n"
                << "  end\n"
                << "Default admin account: admin / admin\n";
            return;
        }

        currentUser->help(std::cout);
    }

    bool Calorix::saveToFile() const {
        std::ofstream out(dataFileName);
        if (!out) {
            std::cout << "Could not open file for saving: " << dataFileName << '\n';
            return false;
        }

        out << "CALORIX_V1\n";
        out << "COUNTERS " << nextUserId << ' ' << nextFoodId << ' ' << nextExerciseId << ' ' << nextEntryId << "\n";

        out << "FOODS " << foods.size() << "\n";
        for (const auto& food : foods) {
            out << food->getFoodId() << ' '
                << food->getName() << ' '
                << food->getCaloriesPer100g() << ' '
                << food->getProteinPer100g() << ' '
                << food->getCarbsPer100g() << ' '
                << food->getFatPer100g() << "\n";
        }

        out << "EXERCISES " << exercises.size() << "\n";
        for (const auto& exercise : exercises) {
            out << exercise->getExerciseId() << ' '
                << exercise->getName() << ' '
                << exercise->getCaloriesBurnedPerHour() << ' '
                << toString(exercise->getMuscleGroup()) << ' '
                << exercise->getDurationMinutes() << "\n";
        }

        out << "USERS " << users.size() << "\n";
        for (const auto& user : users) {
            const UserProfile& profile = user->getProfile();
            out << "USER " << user->getRole() << ' '
                << user->getUserId() << ' '
                << user->getUsername() << ' '
                << user->getPassword() << ' '
                << profile.getAge() << ' '
                << profile.getWeight() << ' '
                << profile.getHeight() << ' '
                << toString(profile.getGender()) << ' '
                << toString(profile.getActivityLevel()) << "\n";

            if (const Trainee* trainee = dynamic_cast<const Trainee*>(user.get())) {
                const FitnessGoal& goal = trainee->getGoal();
                out << "GOAL "
                    << toString(goal.getGoalType()) << ' '
                    << goal.getTargetValue() << ' '
                    << goal.getStartDate().toString() << ' '
                    << goal.getDeadline().toString() << ' '
                    << (goal.isAchieved() ? 1 : 0) << "\n";

                const auto& foodDiary = trainee->getFoodDiary();
                out << "FOOD_DIARY " << foodDiary.size() << "\n";
                for (const FoodEntry& entry : foodDiary) {
                    out << entry.getEntryId() << ' '
                        << (entry.getFood() ? entry.getFood()->getFoodId() : 0) << ' '
                        << entry.getQuantityGrams() << ' '
                        << entry.getDate().toString() << "\n";
                }

                const auto& exerciseDiary = trainee->getExerciseDiary();
                out << "EXERCISE_DIARY " << exerciseDiary.size() << "\n";
                for (const ExerciseEntry& entry : exerciseDiary) {
                    out << entry.getEntryId() << ' '
                        << (entry.getExercise() ? entry.getExercise()->getExerciseId() : 0) << ' '
                        << entry.getDurationMinutes() << ' '
                        << entry.getDate().toString() << "\n";
                }

                const auto& favorites = trainee->getFavoriteExercises();
                out << "FAVORITES " << favorites.size() << "\n";
                for (const Exercise* exercise : favorites) {
                    out << (exercise ? exercise->getExerciseId() : 0) << "\n";
                }
            }

            out << "END_USER\n";
        }

        return true;
    }

    bool Calorix::loadFromFile() {
        std::ifstream in(dataFileName);
        if (!in) {
            createDefaultAdmin();
            return false;
        }

        users.clear();
        foods.clear();
        exercises.clear();
        currentUser = nullptr;

        std::string marker;
        in >> marker;
        if (marker != "CALORIX_V1") {
            createDefaultAdmin();
            return false;
        }

        in >> marker;
        if (marker != "COUNTERS") {
            createDefaultAdmin();
            return false;
        }
        in >> nextUserId >> nextFoodId >> nextExerciseId >> nextEntryId;

        size_t count = 0;

        in >> marker >> count;
        if (marker != "FOODS") {
            createDefaultAdmin();
            return false;
        }
        for (size_t i = 0; i < count; ++i) {
            unsigned id = 0;
            std::string name;
            double calories = 0;
            double protein = 0;
            double carbs = 0;
            double fat = 0;
            in >> id >> name >> calories >> protein >> carbs >> fat;
            foods.push_back(std::make_unique<Food>(id, name, calories, protein, carbs, fat));
        }

        in >> marker >> count;
        if (marker != "EXERCISES") {
            createDefaultAdmin();
            return false;
        }
        for (size_t i = 0; i < count; ++i) {
            unsigned id = 0;
            std::string name;
            double calories = 0;
            std::string groupText;
            unsigned duration = 30;
            MuscleGroup group = MuscleGroup::CARDIO;

            in >> id >> name >> calories >> groupText >> duration;
            parseMuscleGroup(groupText, group);
            exercises.push_back(std::make_unique<Exercise>(id, name, calories, group, duration));
        }

        in >> marker >> count;
        if (marker != "USERS") {
            createDefaultAdmin();
            return false;
        }

        for (size_t i = 0; i < count; ++i) {
            std::string userKeyword;
            std::string role;
            unsigned id = 0;
            std::string username;
            std::string password;
            unsigned age = 0;
            double weight = 0;
            double height = 0;
            std::string genderText;
            std::string activityText;
            Gender gender = Gender::FEMALE;
            ActivityLevel activityLevel = ActivityLevel::MODERATE;

            in >> userKeyword >> role >> id >> username >> password >> age >> weight >> height >> genderText >> activityText;

            parseGender(genderText, gender);
            parseActivityLevel(activityText, activityLevel);

            UserProfile profile(age, weight, height, gender, activityLevel);

            if (role == "ADMIN") {
                users.push_back(std::make_unique<Admin>(id, username, password, profile));
                in >> marker; // END_USER
            }
            else if (role == "TRAINEE") {
                auto trainee = std::make_unique<Trainee>(id, username, password, profile);

                std::string goalTypeText;
                double targetValue = 0;
                std::string startDateText;
                std::string deadlineText;
                int achieved = 0;
                GoalType goalType = GoalType::MAINTENANCE;
                Date startDate;
                Date deadline;

                in >> marker >> goalTypeText >> targetValue >> startDateText >> deadlineText >> achieved;
                if (marker == "GOAL") {
                    parseGoalType(goalTypeText, goalType);
                    Date::parse(startDateText, startDate);
                    Date::parse(deadlineText, deadline);
                    FitnessGoal goal(goalType, targetValue, startDate, deadline);
                    goal.setAchieved(achieved != 0);
                    trainee->setGoals(goal);
                }

                size_t entriesCount = 0;
                in >> marker >> entriesCount;
                if (marker == "FOOD_DIARY") {
                    for (size_t j = 0; j < entriesCount; ++j) {
                        unsigned entryId = 0;
                        unsigned foodId = 0;
                        double grams = 0;
                        std::string dateText;
                        Date date;

                        in >> entryId >> foodId >> grams >> dateText;
                        Date::parse(dateText, date);
                        trainee->addLoadedFoodEntry(FoodEntry(entryId, findFoodById(foodId), grams, date));
                    }
                }

                in >> marker >> entriesCount;
                if (marker == "EXERCISE_DIARY") {
                    for (size_t j = 0; j < entriesCount; ++j) {
                        unsigned entryId = 0;
                        unsigned exerciseId = 0;
                        unsigned duration = 0;
                        std::string dateText;
                        Date date;

                        in >> entryId >> exerciseId >> duration >> dateText;
                        Date::parse(dateText, date);
                        trainee->addLoadedExerciseEntry(ExerciseEntry(entryId, findExerciseById(exerciseId), duration, date));
                    }
                }

                in >> marker >> entriesCount;
                if (marker == "FAVORITES") {
                    for (size_t j = 0; j < entriesCount; ++j) {
                        unsigned exerciseId = 0;
                        in >> exerciseId;
                        trainee->addLoadedFavorite(findExerciseById(exerciseId));
                    }
                }

                in >> marker; 
                users.push_back(std::move(trainee));
            }
        }

        createDefaultAdmin();
        return true;
    }

} 