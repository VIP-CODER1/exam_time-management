#include <bits/stdc++.h>
#include <fstream>
using namespace std;

string getCurrentTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buffer);
}

class Exam {
public:
    string subject;
    string date;
    string startTime;
    int duration; 
    string endTime;
    string status;

    Exam(string _subject, string _date, string _startTime, int _duration)
        : subject(_subject), date(_date), startTime(_startTime), duration(_duration), status("Scheduled") {
        calculateEndTime();
    }

    void calculateEndTime() {
        int hour = stoi(startTime.substr(0, 2));
        int minute = stoi(startTime.substr(3, 2));

        minute += duration;
        hour += minute / 60;
        minute = minute % 60;

        endTime = (hour < 10 ? "0" : "") + to_string(hour) + ":" + (minute < 10 ? "0" : "") + to_string(minute);
    }

    bool isOverlapping(const Exam& other) const {
        if (date != other.date) return false;
        return (startTime < other.endTime && endTime > other.startTime);
    }

    void displayExamInfo() const {
        cout << "Subject: " << subject << "\nDate: " << date << "\nStart Time: " << startTime
             << "\nEnd Time: " << endTime << "\nDuration: " << duration << " minutes"
             << "\nStatus: " << status << "\n";
    }

    void updateStatus(const string& newStatus) {
        status = newStatus;
    }
};

// ExamTimeManagementSystem class to manage exams
class ExamTimeManagementSystem {
private:
    vector<Exam> exams;
    map<string, vector<string>> examHistory; // Stores exam results history

    void autoResolveConflicts(Exam& newExam) {
        for (auto& exam : exams) {
            if (newExam.isOverlapping(exam)) {
                cout << "Conflict detected with exam: " << exam.subject << "\n";
                // Attempt to reschedule conflicting exam
                auto newTime = getNewTimeSlot();
                exam.startTime = newTime.first;
                exam.calculateEndTime();
                cout << "Rescheduled exam: " << exam.subject << " to new time: " << exam.startTime << "\n";
            }
        }
    }

    pair<string, string> getNewTimeSlot() {
        string newStartTime = "14:00"; 
        return {newStartTime, "16:00"}; 
    }

public:
    void scheduleExam(const string& subject, const string& date, const string& startTime, int duration) {
        Exam newExam(subject, date, startTime, duration);
        autoResolveConflicts(newExam); 

        exams.push_back(newExam);
        cout << "Exam scheduled successfully.\n";
        logExamHistory(subject, "Scheduled");
    }

    void displayAllExams() const {
        if (exams.empty()) {
            cout << "No exams scheduled.\n";
            return;
        }

        for (const auto& exam : exams) {
            exam.displayExamInfo();
            cout << "--------------------------------\n";
        }
    }

    void updateExamStatus(const string& subject, const string& newStatus) {
        for (auto& exam : exams) {
            if (exam.subject == subject) {
                exam.updateStatus(newStatus);
                cout << "Status updated successfully for " << subject << ".\n";
                logExamHistory(subject, newStatus);
                return;
            }
        }
        cout << "Exam not found!\n";
    }

    void setReminder(const string& subject, int minutesBefore) {
        for (const auto& exam : exams) {
            if (exam.subject == subject) {
                cout << "Reminder set for " << subject << " " << minutesBefore << " minutes before the exam.\n";
                // Simulating a reminder (In real application, use threading or scheduling library)
                this_thread::sleep_for(chrono::seconds(5)); // Just a simulation of a reminder after 5 seconds
                cout << "Reminder: Your exam for " << subject << " is scheduled soon!\n";
                return;
            }
        }
        cout << "Exam not found!\n";
    }

    void generatePerformanceReport() const {
        cout << "Exam Performance Report:\n";
        for (const auto& exam : exams) {
            cout << "Subject: " << exam.subject << ", Status: " << exam.status << "\n";
        }
    }

    void logExamHistory(const string& subject, const string& status) {
        examHistory[subject].push_back(status);
    }

    void displayExamHistory() const {
        if (examHistory.empty()) {
            cout << "No exam history available.\n";
            return;
        }

        cout << "Exam History:\n";
        for (const auto& entry : examHistory) {
            cout << "Subject: " << entry.first << "\nHistory:\n";
            for (const auto& status : entry.second) {
                cout << " - " << status << "\n";
            }
        }
    }

    void loadExamsFromCSV(const string& fileName) {
        ifstream file(fileName);
        if (!file.is_open()) {
            cout << "Error opening file " << fileName << "\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string subject, date, startTime, durationStr;
            int duration;

            getline(ss, subject, ',');
            getline(ss, date, ',');
            getline(ss, startTime, ',');
            getline(ss, durationStr, ',');

            duration = stoi(durationStr);
            scheduleExam(subject, date, startTime, duration);
        }
        file.close();
        cout << "Exams loaded successfully from " << fileName << "\n";
    }
};

// Main function
int main() {
    ExamTimeManagementSystem examSystem;
    int choice;

    do {
        cout << "\nAdvanced Exam Time Management System\n";
        cout << "1. Schedule Exam\n";
        cout << "2. View All Exams\n";
        cout << "3. Update Exam Status\n";
        cout << "4. Set Reminder\n";
        cout << "5. Generate Performance Report\n";
        cout << "6. View Exam History\n";
        cout << "7. Load Exams from CSV\n";  // New menu option
        cout << "8. Exit\n";  // Updated exit option
        cout << "Enter your choice: ";
        
        cin >> choice;

        switch (choice) {
        case 1: {
            string subject, date, startTime;
            int duration;
            cout << "Enter Subject: ";
            cin >> subject;
            cout << "Enter Date (YYYY-MM-DD): ";
            cin >> date;
            cout << "Enter Start Time (HH:MM): ";
            cin >> startTime;
            cout << "Enter Duration (minutes): ";
            cin >> duration;
            examSystem.scheduleExam(subject, date, startTime, duration);
            break;
        }
        case 2:
            examSystem.displayAllExams();
            break;
        case 3: {
            string subject, status;
            cout << "Enter Subject: ";
            cin >> subject;
            cout << "Enter New Status (Completed/Missed/Rescheduled): ";
            cin >> status;
            examSystem.updateExamStatus(subject, status);
            break;
        }
        case 4: {
            string subject;
            int minutesBefore;
            cout << "Enter Subject: ";
            cin >> subject;
            cout << "Enter Reminder Time (minutes before the exam): ";
            cin >> minutesBefore;
            examSystem.setReminder(subject, minutesBefore);
            break;
        }
        case 5:
            examSystem.generatePerformanceReport();
            break;
        case 6:
            examSystem.displayExamHistory();
            break;
        case 7: {
            string fileName;
            cout << "Enter CSV file name: ";
            cin >> fileName;
            examSystem.loadExamsFromCSV(fileName);
            break;
        }
        case 8:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 8);

    return 0;
}
