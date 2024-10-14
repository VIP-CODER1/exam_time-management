#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include<set>
#include <algorithm> // Include algorithm for std::shuffle
#include <random> // for random generation
#include <hpdf.h>  // Include libharu for PDF generation
#include <chrono> // for seeding randomness

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
    string semester;
    string date;
    string startTime;   
    string duration;
   string endTime;
string status; 
    Exam(string subj, string sem, string dt, string start, string end, string dur, string stat)
        : subject(subj), semester(sem), date(dt), startTime(start), endTime(end), duration(dur), status(stat) {}
};


class Room {
public:
    string roomNumber;
    int capacity;

    Room(string roomNo, int cap) : roomNumber(roomNo), capacity(cap) {}
};





class Invigilator {
public:
    string date;
    string day;
    string roomNumber;
    vector<string> invigilators;

    Invigilator(string dt, string d, string roomNo, const vector<string>& invigilatorsList)
        : date(dt), day(d), roomNumber(roomNo), invigilators(invigilatorsList) {}
};

class ExamTimeManagementSystem {
public:
    vector<Exam> exams;
    vector<Room> rooms;
    vector<Invigilator> invigilators;
    
public:
    // Schedule an exam
   
void scheduleExam(string subject, string semester, string date, string startTime, int duration) {
    // Check if the exam with the same subject, date, and start time already exists
    for (const auto& exam : exams) {
        if (exam.subject == subject && exam.date == date && exam.startTime == startTime) {
            cout << "Exam already scheduled for the same subject, date, and time!" << endl;
            return;  // Exit the function, no duplicate scheduling
        }
    }

    // Split startTime into hours and minutes
    int startHours, startMinutes;
    sscanf(startTime.c_str(), "%d:%d", &startHours, &startMinutes);
    
    // Calculate end time
    int totalMinutes = startHours * 60 + startMinutes + duration;
    int endHours = (totalMinutes / 60) % 24; // Wrap around if > 24
    int endMinutes = totalMinutes % 60;

    // Format the endTime as "HH:MM"
    string endTime = to_string(endHours) + ":" + (endMinutes < 10 ? "0" : "") + to_string(endMinutes);

    // Store the duration and status
    string examDuration = to_string(duration) + " mins";
    string status = "Scheduled";

    // Add the new exam to the list
    exams.push_back(Exam(subject, semester, date, startTime, endTime, examDuration, status));

    // Inform the user about successful scheduling
    cout << "Exam scheduled successfully!" << endl;
}


    

    // Display exam details with boundary
    string displayAllExamsInOnePageWithBoundary() const {
        if (exams.empty()) {
            return "No exams scheduled.\n";
        }

        stringstream ss;

        ss << string(25, ' ') << string(84, '-') << "\n";  // Top border
        ss << string(25, ' ') << "| " << setw(81) << left << "भारतीय सूचना प्रौद्योगिकी संस्थान सेनापति, मणिपुर" << string(40, ' ') << "|\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "INDIAN INSTITUTE OF INFORMATION TECHNOLOGY SENAPATI, MANIPUR" << " |\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "(An Institute of National Importance by Act of Parliament, Government of India)" << " |\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "Mantripukhri, Imphal- 795002, Manipur, India, www.iiitmanipur.ac.in" << " |\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "ASSESSMENT II TIME TABLE for B.TECH 3RD, 5TH and 7TH SEMESTERS" << " |\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "Timing: 8:00 – 8:50 AM; Reporting Time: 7:50 AM" << "   |\n";

        ss << string(25, ' ') << string(84, '-') << "\n";  // Bottom border

        ss << string(133, '*') << "\n";  // Top boundary
        ss << "* " << left << setw(30) << "Subject" 
           << setw(20) << "Semester"
           << setw(20) << "Date"
           << setw(15) << "Start Time"
           << setw(15) << "End Time"
           << setw(15) << "Duration"
           << setw(15) << "Status"
           << "*\n";
        ss << string(133, '-') << "\n";  // Header boundary

        string lastDate = "";  // Track the last printed date

        for (const auto& exam : exams) {
            // Print the exam details
            ss << "* " << left << setw(30) << exam.subject
               << setw(20) << exam.semester
               << setw(20) << exam.date
               << setw(15) << exam.startTime
               << setw(15) << exam.endTime
               << setw(15) << exam.duration
               << setw(15) << exam.status
               << "*\n";

            // Check if the date has changed, then add '*' separator for completed day
            if (lastDate != exam.date) {
                if (!lastDate.empty()) {
                    // After printing all exams of a day, add a separator
                    ss << string(133, '*') << "\n";
                }
                lastDate = exam.date;  // Update the last date
            }
        }

        // Add the final boundary after the last exam of the last day
        ss << string(133, '*') << "\n";  // Bottom boundary

        return ss.str();  // Return the stringstream content as a string
    }

    // Update exam status
    void updateExamStatus(string subject, string newStatus) {
        for (auto& exam : exams) {
            if (exam.subject == subject) {
                exam.status = newStatus;
                cout << "Status of " << subject << " updated to " << newStatus << ".\n";
                return;
            }
        }
        cout << "Exam not found.\n";
    }

    // Convert the captured content to PDF format
    

void convertToPDF(const string& filename, const string& content) const {
    // Create a new PDF document
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        cout << "Error: Cannot create PDF document\n";
        return;
    }

    // Set the page size to A4
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    // Use a monospaced font to ensure alignment
    HPDF_Font font = HPDF_GetFont(pdf, "Courier", NULL);
    HPDF_Page_SetFontAndSize(page, font, 7); // Adjust font size as needed

    // Define initial text position
    float textPosY = 770;
    const float marginLeft = 50;

    // Split the content by lines
    stringstream ss(content);
    string line;

    while (getline(ss, line)) {
        // Check if the content exceeds the page size
        if (textPosY < 100) {
            // Add a new page if content exceeds page size
            page = HPDF_AddPage(pdf);
            HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
            HPDF_Page_SetFontAndSize(page, font, 7); // Set the same font on the new page
            textPosY = 770; // Reset position for the new page
        }

        // Begin text and move to position
        HPDF_Page_BeginText(page);
        HPDF_Page_MoveTextPos(page, marginLeft, textPosY);

        // Show text on the page
        HPDF_Page_ShowText(page, line.c_str());
        HPDF_Page_EndText(page);

        // Move down for the next line
        textPosY -= 15; // Adjust line height as needed
    }

    // Save the PDF to the file
    HPDF_SaveToFile(pdf, filename.c_str());

    // Clean up
    HPDF_Free(pdf);

    cout << "PDF generated successfully as " << filename << "!\n";
}

    
    // Function to convert seating arrangement to PDF
void convertSeatingToPDF(const string& filename, const string& seatingArrangement) const {
    // Create a new PDF document
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        cout << "Error: Cannot create PDF document\n";
        return;
    }

    // Add a page to the PDF
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    // Set font and size
    HPDF_Font font = HPDF_GetFont(pdf, "Courier", NULL);
    HPDF_Page_SetFontAndSize(page, font, 7);

    // Define initial text position
    float textPosY = 770;
    const float marginLeft = 50;

    // Split the seating arrangement by lines
    stringstream ss(seatingArrangement);
    string line;

    while (getline(ss, line)) {
        if (textPosY < 100) {
            // Add a new page if content exceeds page size
            page = HPDF_AddPage(pdf);
            HPDF_Page_SetFontAndSize(page, font, 7);
            textPosY = 770;
        }

        // Print line to PDF
        HPDF_Page_BeginText(page);
        HPDF_Page_MoveTextPos(page, marginLeft, textPosY);
        HPDF_Page_ShowText(page, line.c_str());
        HPDF_Page_EndText(page);

        textPosY -= 20; // Move down for the next line
    }

    // Save the PDF to the file
    HPDF_SaveToFile(pdf, filename.c_str());

    // Clean up
    HPDF_Free(pdf);

    cout << "Seating arrangement PDF generated successfully as " << filename << "!\n";
}



    // Load exams from CSV file
    void loadExamsFromCSV(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open file " << filename << "\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string subject, semester, date, startTime;
            int duration;
            getline(ss, subject, ',');
            getline(ss, semester, ',');
            getline(ss, date, ',');
            getline(ss, startTime, ',');
            ss >> duration;

            scheduleExam(subject, semester, date, startTime, duration);
        }

        cout << "Exams loaded successfully from " << filename << "\n";
        file.close();
    }
    

    // Load room seating from CSV file
    void loadRoomSeatingFromCSV(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open file " << filename << "\n";
            return;
        }
 
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string roomNumber;
            int capacity;
            getline(ss, roomNumber, ',');
            ss >> capacity;

            rooms.push_back(Room(roomNumber, capacity));
        }

        cout << "Room seating loaded successfully from " << filename << "\n";
        file.close();
    }

    // Display room seating details
   string displayRoomSeatingWithBoundary() const {
    if (rooms.empty()) {
        return "No rooms available for seating arrangement.\n";
    }

    stringstream ss;


        ss << string(25, ' ') << string(84, '-') << "\n";  // Top border
        ss << string(25, ' ') << "| " << setw(81) << left << "भारतीय सूचना प्रौद्योगिकी संस्थान सेनापति, मणिपुर" << string(40, ' ') << "|\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "INDIAN INSTITUTE OF INFORMATION TECHNOLOGY SENAPATI, MANIPUR" << " |\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "(An Institute of National Importance by Act of Parliament, Government of India)" << " |\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "Mantripukhri, Imphal- 795002, Manipur, India, www.iiitmanipur.ac.in" << " |\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "ASSESSMENT II TIME TABLE for B.TECH 3RD, 5TH and 7TH SEMESTERS" << " |\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "Timing: 8:00 – 8:50 AM; Reporting Time: 7:50 AM" << "   |\n";

        ss << string(25, ' ') << string(84, '-') << "\n";  // Bottom border

    ss << string(53, '*') << "\n";  // Top boundary
    ss << "* " << left << setw(30) << "Room Number" 
       << setw(53) << " " // Add space to fill width
       << "*\n";
    ss << string(53, '-') << "\n";  // Header boundary

    for (const auto& room : rooms) {
        // Print the room details
        ss << "* " << left << setw(30) << room.roomNumber
           //<< setw(20) << room.capacity
           << setw(5) << " "
           << "*\n";
    }

    // Add the final boundary after the last room
    ss << string(53, '*') << "\n";  // Bottom boundary

    return ss.str();  // Return the stringstream content as a string
}

 // Load invigilator duties from a CSV file

 // Load invigilator duties from a CSV file
void loadInvigilatorDuties(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Failed to open invigilator duties file: " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string date, day, roomNumber, invigilator;
            vector<string> invigilatorsList;

            getline(ss, date, ',');
            getline(ss, day, ',');
            getline(ss, roomNumber, ',');

            while (getline(ss, invigilator, ',')) {
                invigilatorsList.push_back(invigilator);
            }

            invigilators.push_back(Invigilator(date, day, roomNumber, invigilatorsList)); 

        }

        file.close();
        cout << "Invigilator duties loaded successfully from " << filename << "!\n";
    }
    // Display invigilator duties details
   string displayInvigilatorDutiesWithBoundary() const {
    if (this->invigilators.empty()) {
        return "No invigilator duties available.\n";
    }

    stringstream ss;


        ss << string(25, ' ') << string(84, '-') << "\n";  // Top border
        ss << string(25, ' ') << "| " << setw(81) << left << "भारतीय सूचना प्रौद्योगिकी संस्थान सेनापति, मणिपुर" << string(40, ' ') << "|\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "INDIAN INSTITUTE OF INFORMATION TECHNOLOGY SENAPATI, MANIPUR" << " |\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "(An Institute of National Importance by Act of Parliament, Government of India)" << " |\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "Mantripukhri, Imphal- 795002, Manipur, India, www.iiitmanipur.ac.in" << " |\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "ASSESSMENT II TIME TABLE for B.TECH 3RD, 5TH and 7TH SEMESTERS" << " |\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "Timing: 8:00 – 8:50 AM; Reporting Time: 7:50 AM" << "   |\n";

        ss << string(25, ' ') << string(84, '-') << "\n";  // Bottom border

    ss << string(143, '*') << "\n";  // Top boundary
    ss << "* " << left << setw(30) << "Date" 
       << setw(15) << "Day"
       << setw(30) << "Room Number"
       << setw(25) << "Invigilators"
       << "\n";
    ss << string(143, '-') << "\n";  // Header boundary

    for (const auto& duty : invigilators) {
        // Print the invigilator duty details
        ss << " *" << left << setw(30) << duty.date
           << setw(15) << duty.day
           << setw(30) << duty.roomNumber
           << setw(25) << [duty] {
               string names;
               for (const auto& name : duty.invigilators) {
                   names += name + ", "; // Concatenate names with comma
               }
               if (!names.empty()) {
                   names.pop_back(); // Remove last comma
                   names.pop_back(); // Remove last space
               }
               return names; // Return concatenated names
           }()  // Lambda function to print invigilators
          <<right << setw(5) << " "<< "\n";
       
    }

    // Add the final boundary after the last duty
    ss << string(143, '*') << "\n";  // Bottom boundary

    return ss.str();  // Return the stringstream content as a string
}

// // Generate PDF for invigilator duties

void convertInvigilatorDutiesToPDF(const string& filename) const {
    // Create a new PDF document
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        cout << "Error: Cannot create PDF document\n";
        return;
    }
    
    // Add a page to the PDF
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    // Set font and size
    HPDF_Font font = HPDF_GetFont(pdf, "Courier", NULL);
    HPDF_Page_SetFontAndSize(page, font, 8);

    // Define initial text position
    float textPosY = 770;
    const float marginLeft = 50;

    // Create the header using stringstream
    std::stringstream ss;
    ss << std::string(25, ' ') << std::string(84, '-') << "\n";  // Top border
    ss << std::string(25, ' ') << "| " << std::setw(81) << std::left << "भारतीय सूचना प्रौद्योगिकी संस्थान सेनापति, मणिपुर" << std::string(40, ' ') << "|\n";
    ss << std::string(25, ' ') << "| " << std::setw(81) << std::left << "INDIAN INSTITUTE OF INFORMATION TECHNOLOGY SENAPATI, MANIPUR" << " |\n";
    ss << std::string(25, ' ') << "| " << std::setw(81) << std::left << "(An Institute of National Importance by Act of Parliament, Government of India)" << " |\n";
    ss << std::string(25, ' ') << "| " << std::setw(81) << std::left << "Mantripukhri, Imphal- 795002, Manipur, India, www.iiitmanipur.ac.in" << " |\n";
    ss << std::string(25, ' ') << "| " << std::setw(81) << std::left << "ASSESSMENT II TIME TABLE for B.TECH 3RD, 5TH and 7TH SEMESTERS" << " |\n";
    ss << std::string(25, ' ') << "| " << std::setw(81) << std::left << "Timing: 8:00 – 8:50 AM; Reporting Time: 7:50 AM" << "   |\n";
    ss << std::string(25, ' ') << std::string(84, '-') << "\n";  // Bottom border

    // Print each line of the header
    std::string line;
    while (std::getline(ss, line)) {
        HPDF_Page_BeginText(page);
        HPDF_Page_MoveTextPos(page, marginLeft, textPosY);
        HPDF_Page_ShowText(page, line.c_str());  // Output the line to the PDF
        HPDF_Page_EndText(page);
        textPosY -= 20;  // Move down for the next line
    }

    // Print the invigilator duties to PDF
    for (const auto& duty : invigilators) {
        // Check if the content exceeds the page size
        if (textPosY < 100) {
            page = HPDF_AddPage(pdf);
            HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
            HPDF_Page_SetFontAndSize(page, font, 8);
            textPosY = 770; // Reset position for the new page
        }

        // Prepare invigilators string
        std::string invigilatorsList;
        for (const auto& name : duty.invigilators) {
            invigilatorsList += name + ", ";  // Concatenate names with a comma
        }
        if (!invigilatorsList.empty()) {
            invigilatorsList.pop_back();  // Remove last comma
            invigilatorsList.pop_back();  // Remove last space
        }

        // Begin text for the invigilator duty
        HPDF_Page_BeginText(page);
        HPDF_Page_MoveTextPos(page, marginLeft, textPosY);
        HPDF_Page_ShowText(page, (duty.date + " | " + duty.day + " | " + duty.roomNumber + " | " + invigilatorsList).c_str());
        HPDF_Page_EndText(page);

        textPosY -= 20;  // Move down for the next line
    }

    // Save the PDF to the file
    HPDF_SaveToFile(pdf, filename.c_str());

    // Clean up
    HPDF_Free(pdf);

    cout << "Invigilator duties PDF generated successfully as " << filename << "!\n";
}

    
    //Generate the random exam shedule
    
   // Function to generate random date in a specific range
string generateRandomDate(const string& startDate, const string& endDate) {
    // Parse the startDate and endDate (assumed format: YYYY-MM-DD)
    tm startTm = {}, endTm = {};
    istringstream ssStart(startDate), ssEnd(endDate);
    ssStart >> get_time(&startTm, "%Y-%m-%d");
    ssEnd >> get_time(&endTm, "%Y-%m-%d");

    // Convert to time_t
    time_t start = mktime(&startTm);
    time_t end = mktime(&endTm);

    // Generate a random date between start and end
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(start, end);
    time_t randomTime = dist(gen);

    // Convert back to tm and then to string
    tm* randomTm = localtime(&randomTime);
    stringstream ss;
    ss << put_time(randomTm, "%Y-%m-%d");
    return ss.str();
}

// Modified generateRandomExamSchedule function
void generateRandomExamSchedule() {
    if (exams.empty()) {
        cout << "No exam data available. Please load exams from the CSV first.\n";
        return;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, exams.size() - 1);
    char regenerateChoice;

    // Define the date range for randomization (customize as needed)
    string startDate = "2024-10-28"; // Starting date of the exam schedule
    string endDate = "2024-11-8";    // Ending date of the exam schedule

    do {
        vector<Exam> randomExams;  // Store random exams for this schedule
        set<int> usedIndices;      // Track indices that have already been selected
        stringstream ss;

        // Format the header as in the displayAllExamsInOnePageWithBoundary function
        ss << string(25, ' ') << string(84, '-') << "\n";  // Top border
        ss << string(25, ' ') << "| " << setw(81) << left << "भारतीय सूचना प्रौद्योगिकी संस्थान सेनापति, मणिपुर" << string(40, ' ') << "|\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "INDIAN INSTITUTE OF INFORMATION TECHNOLOGY SENAPATI, MANIPUR" << " |\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "(An Institute of National Importance by Act of Parliament, Government of India)" << " |\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "Mantripukhri, Imphal- 795002, Manipur, India, www.iiitmanipur.ac.in" << " |\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "ASSESSMENT II TIME TABLE for B.TECH 3RD, 5TH and 7TH SEMESTERS" << " |\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "Timing: 8:00 – 8:50 AM; Reporting Time: 7:50 AM" << "   |\n";
        ss << string(25, ' ') << string(84, '-') << "\n";  // Bottom border

        ss << string(133, '*') << "\n";  // Top boundary
        ss << "* " << left << setw(30) << "Subject" 
           << setw(20) << "Semester"
           << setw(20) << "Date"
           << setw(15) << "Start Time"
           << setw(15) << "End Time"
           << setw(15) << "Duration"
           << setw(15) << "Status"
           << "*\n";
        ss << string(133, '-') << "\n";  // Header boundary

        // Generate random exams
        while (randomExams.size() < exams.size()) {
            int randomIndex = dist(gen);

            // Ensure the exam at randomIndex hasn't been used yet
            if (usedIndices.find(randomIndex) == usedIndices.end()) {
                usedIndices.insert(randomIndex);  // Mark this index as used
                Exam exam = exams[randomIndex];   // Get the exam
                
                // Randomize the exam date
                exam.date = generateRandomDate(startDate, endDate);
                
                randomExams.push_back(exam);  // Add the exam to the random schedule

                // Format the exam details for the output
                ss << "* " << left << setw(30) << exam.subject
                   << setw(20) << exam.semester
                   << setw(20) << exam.date
                   << setw(15) << exam.startTime
                   << setw(15) << exam.endTime
                   << setw(15) << exam.duration
                   << setw(15) << exam.status
                   << "*\n";
            }
        }

        // Add the final boundary after the last exam
        ss << string(133, '*') << "\n";  // Bottom boundary

        // Display the generated random schedule
        cout << ss.str();

        // Ask the user if they want to export the schedule to PDF
        char exportChoice;
        cout << "Do you want to export this schedule to PDF? (y/n): ";
        cin >> exportChoice;

        if (exportChoice == 'y' || exportChoice == 'Y') {
            string pdfFileName;
            cout << "Enter PDF file name to save: ";
            cin >> pdfFileName;

            // Convert the formatted schedule to PDF
            convertToPDF(pdfFileName, ss.str());
        }

        // Ask the user if they want to regenerate the exam schedule
        cout << "Do you want to regenerate the exam schedule? (y/n): ";
        cin >> regenerateChoice;

    } while (regenerateChoice == 'y' || regenerateChoice == 'Y');
}

void shuffleInvigilators(vector<string>& invigilatorsList) {
    // Use a time-based seed for randomness
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(invigilatorsList.begin(), invigilatorsList.end(), default_random_engine(seed));
}


// Generate random invigilator duties based on available invigilators
void generateRandomInvigilatorDuties() {
    if (invigilators.empty()) {
        cout << "No invigilators available to assign.\n";
        return;
    }
   stringstream ss;
    // Format the header as in the displayAllExamsInOnePageWithBoundary function
        ss << string(25, ' ') << string(84, '-') << "\n";  // Top border
        ss << string(25, ' ') << "| " << setw(81) << left << "भारतीय सूचना प्रौद्योगिकी संस्थान सेनापति, मणिपुर" << string(40, ' ') << "|\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "INDIAN INSTITUTE OF INFORMATION TECHNOLOGY SENAPATI, MANIPUR" << " |\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "(An Institute of National Importance by Act of Parliament, Government of India)" << " |\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "Mantripukhri, Imphal- 795002, Manipur, India, www.iiitmanipur.ac.in" << " |\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "ASSESSMENT II TIME TABLE for B.TECH 3RD, 5TH and 7TH SEMESTERS" << " |\n";
        ss << string(25, ' ') << "| " << setw(81) << left << "Timing: 8:00 – 8:50 AM; Reporting Time: 7:50 AM" << "   |\n";
        ss << string(25, ' ') << string(84, '-') << "\n";  // Bottom border

        ss << string(133, '*') << "\n";  // Top boundary
       
        ss << string(133, '-') << "\n";  // Header boundary
   
    // Flatten all invigilator names into a single list
    vector<string> allInvigilators;
    for (const auto& invigilator : invigilators) {
        allInvigilators.insert(allInvigilators.end(), invigilator.invigilators.begin(), invigilator.invigilators.end());
    }

    // Shuffle the invigilators for random assignment
    shuffleInvigilators(allInvigilators);

    size_t invigilatorIndex = 0;

    // Randomly assign invigilators to each duty
    for (auto& duty : invigilators) {
        vector<string> assignedInvigilators;
        int numInvigilatorsRequired = duty.invigilators.size(); // Get how many invigilators are needed for this duty

        // Assign invigilators in a round-robin fashion from the shuffled list
        for (int i = 0; i < numInvigilatorsRequired; ++i) {
            assignedInvigilators.push_back(allInvigilators[invigilatorIndex % allInvigilators.size()]);
            invigilatorIndex++;
        }

        // Update the invigilator list for this duty with random invigilators
        duty.invigilators = assignedInvigilators;
    }
     // Display the generated random schedule
        cout << displayInvigilatorDutiesWithBoundary();
    
    //ask user to save as pdf
char exportChoice;
    cout << "Do you want to export this schedule to PDF? (y/n): ";
    cin >> exportChoice;

    if (exportChoice == 'y' || exportChoice == 'Y') {
        string pdfInvigilatorFileName;
        cout << "Enter PDF file name to save: ";
        cin >> pdfInvigilatorFileName;

        // Call the member function to convert the schedule to PDF
        this->convertInvigilatorDutiesToPDF(pdfInvigilatorFileName);  // Use 'this' to reference the current object
    }


}




    
};
int main() {
    ExamTimeManagementSystem system;
    int choice;
   
    while (true) {
        cout << "1. Schedule Exam\n"
             << "2. Display All Exams\n"
             << "3. Update Exam Status\n"
             << "4. Generate Performance Report\n"
             << "5. Load Exams from CSV\n"
             << "6. Load Room Seating from CSV\n"
             << "7. Display Room Seating\n"
             << "8. Load Room InvigilatorDuty from CSV\n"
             << "9. Display InvigilatorDuty\n"
             << "10. Regenerate Random Exam Schedule\n"
             << "11. Regenerate Random InvigilatorDuty Schedule\n"
             << "12. Convert to PDF\n"
             << "13. Exit\n"
             << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string subject, semester, date, startTime;
                int duration;
                cout << "Enter subject: ";
                cin >> subject;
                cout << "Enter semester: ";
                cin >> semester;
                cout << "Enter date (YYYY-MM-DD): ";
                cin >> date;
                cout << "Enter start time (HH:MM): ";
                cin >> startTime;
                cout << "Enter duration (in minutes): ";
                cin >> duration;
                system.scheduleExam(subject, semester, date, startTime, duration);
                break;
            }
            case 2: {
                string content = system.displayAllExamsInOnePageWithBoundary();
                cout << content;  // Display exam details in the console
                 
                // Ask the user if they want to export it to PDF
                char exportChoice;
                cout << "Do you want to export this to PDF? (y/n): ";
                cin >> exportChoice;
                if (exportChoice == 'y' || exportChoice == 'Y') {
                    string pdfFileName;
                    cout << "Enter PDF file name to save: ";
                    cin >> pdfFileName;
                    system.convertToPDF(pdfFileName, content);
                }
                break;
            }
            case 3: {
                string subject, newStatus;
                cout << "Enter subject to update: ";
                cin >> subject;
                cout << "Enter new status: ";
                cin >> newStatus;
                system.updateExamStatus(subject, newStatus);
                break;
            }
            case 4:
                cout << "Generate Performance Report feature coming soon.\n";
                break;
            case 5: {
                string filename;
                cout << "Enter the CSV file name for exams: ";
                cin >> filename;
                system.loadExamsFromCSV(filename);
                break;
            }
            case 6: {
                string filename;
                cout << "Enter the CSV file name for room seating: ";
                cin >> filename;
                system.loadRoomSeatingFromCSV(filename);
                break;
            }
            case 7: {
    string content2 = system.displayRoomSeatingWithBoundary();
    cout << content2;

    char exportSeatingChoice;
    cout << "Do you want to export the seating arrangement to PDF? (y/n): ";
    cin >> exportSeatingChoice;
    if (exportSeatingChoice == 'y' || exportSeatingChoice == 'Y') {
        string pdfSeatingFileName;
        cout << "Enter PDF file name to save: ";
        cin >> pdfSeatingFileName;

        // Pass both the filename and the content
        system.convertSeatingToPDF(pdfSeatingFileName, content2);
    }

    break;
}

case 8: {  // Assuming case 8 is for invigilator duties
    string filename;
    cout << "Enter the CSV file name for invigilator duties: ";
    cin >> filename;
    system.loadInvigilatorDuties(filename);
    break;
}
case 9: {  // Assuming case 9 is for displaying invigilator duties
    string content2 = system.displayInvigilatorDutiesWithBoundary();
    cout << content2;

    char exportInvigilatorChoice;
    cout << "Do you want to export the invigilator duties to PDF? (y/n): ";
    cin >> exportInvigilatorChoice;
    if (exportInvigilatorChoice == 'y' || exportInvigilatorChoice == 'Y') {
        string pdfInvigilatorFileName;
        cout << "Enter PDF file name to save: ";
        cin >> pdfInvigilatorFileName;

        // Pass both the filename and the content
        system.convertInvigilatorDutiesToPDF(pdfInvigilatorFileName);
             }

              break;
             }

               case 10:
                system.generateRandomExamSchedule();
                break;
                
                 case 11:
                system.generateRandomInvigilatorDuties();
                
                break;
              
            case 12: {
                string pdfFileName;
                cout << "Enter PDF file name to save: ";
                cin >> pdfFileName;
                string content = system.displayAllExamsInOnePageWithBoundary();
                system.convertToPDF(pdfFileName, content);
                break;
            }
            case 13:
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
