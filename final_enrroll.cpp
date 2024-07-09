#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <cstdlib>

using namespace std;

class EnrollmentError : public std::exception
{
public:
    const char *what() const noexcept override
    {
        return "Enrollment capacity reached for the course.";
    }
};

class Person
{
public:
    string Name;
    int PersonID;

    virtual void display() const = 0;
};

class Course
{
public:
    string courseCode;
    string courseName;
    int courseCredits;
    int courseCapacity;

    queue<int> Waitlist;

    string instructor;
    int level;
    string department;

    // Constructors
    Course(const string &code, const string &name, int credits, int capacity, string inst, int l, string dept)
        : courseCode(code), courseName(name), courseCredits(credits), courseCapacity(capacity), instructor(inst), level(l), department(dept) {}

    // Copy Constructor
    Course(const Course &other)
        : courseCode(other.courseCode), courseName(other.courseName),
          courseCredits(other.courseCredits), courseCapacity(other.courseCapacity), instructor(other.instructor), level(other.level), department(other.department) {}

    string getcode()
    {
        return courseCode;
    }

    string getcname()
    {
        return courseName;
    }

    int get_cred()
    {
        return courseCredits;
    }

    int get_cap()
    {
        return courseCapacity;
    }

    void set_code(string c)
    {
        courseCode = c;
    }

    void set_name(string n)
    {
        courseName = n;
    }

    void set_credits(int r)
    {
        courseCredits = r;
    }

    void set_cap(int cap)
    {
        courseCapacity = cap;
    }

    friend ostream &operator<<(ostream &os, const Course &course)
    {
        os << "Course Code: " << course.courseCode << ", Course Name: " << course.courseName
           << ", Credits: " << course.courseCredits << ", Capacity: " << course.courseCapacity;
        return os;
    }
};

class Student : public Person
{
public:
    int studentID;
    string studentName;
    vector<Course> enrolledCourses;
    vector<Course> droppedCourses;

    // Constructors
    Student(int id, const string &name) : studentID(id), studentName(name) {}

    // Copy Constructor
    Student(const Student &other)
        : Person(other), studentID(other.studentID), studentName(other.studentName),
          enrolledCourses(other.enrolledCourses) {}

    int get_stid()
    {
        return studentID;
    }

    string get_name()
    {
        return studentName;
    }

    void set_id(int i)
    {
        studentID = i;
    }

    void set_name(string n)
    {
        studentName = n;
    }

    void enrollCourse(Course &c)
    {
        auto it = find_if(enrolledCourses.begin(), enrolledCourses.end(),
                          [courseCode = c.courseCode](const Course &course)
                          { return course.courseCode == courseCode; });

        if (it != enrolledCourses.end())
        {
            cout << "You are already enrolled in this course.\n";
            return;
        }
        // Check if the course is full before enrolling
        if (c.Waitlist.empty() && enrolledCourses.size() < c.courseCapacity)
        {
            enrolledCourses.push_back(c);
            cout << "Enrolled in course: " << c.courseCode << endl;
        }
        // course is full, added to waitlist
        else if (c.Waitlist.size() < c.courseCapacity)
        {
            c.Waitlist.push(studentID);
            cout << "Added to waitlist for course: " << c.courseCode << endl;
        }
        // waitlist is full, exception thrown
        else
        {
            cout << "Waitlist is full, cannot be enrolled or added to Waitlist" << endl;
            throw EnrollmentError();
        }
    }

    void display() const override
    {
        for (const auto &course : enrolledCourses)
        {
            cout << course << endl;
        }
    }

  
    void GenerateFee() const
{
    double totalFee = 0.0;
    for (const auto &course : enrolledCourses)
    {
        double courseFee = course.courseCredits * 100.0;
        totalFee += courseFee;
    }
    cout << "Total Fee: $" << totalFee << endl;
}

    void dropCourse()
    {
        // Display enrolled courses
        cout << "Enrolled Courses:\n";
        for (const auto &course : enrolledCourses)
        {
            cout << course << endl;
        }

        // Get the course code to drop
        string courseCode;
        cout << "Enter the Course Code to drop: ";
        cin >> courseCode;

        // Find the course in enrolledCourses and remove it
        auto it = find_if(enrolledCourses.begin(), enrolledCourses.end(),
                          [courseCode](const Course &c)
                          { return c.courseCode == courseCode; });

        if (it != enrolledCourses.end())
        {
            enrolledCourses.erase(it);
            droppedCourses.push_back(*it);
            cout << "Course dropped successfully.\n";
        }
        else
        {
            cout << "Course not found in the list of enrolled courses.\n";
        }
    }
};

class Admin : public Person
{
public:
    int adminID;

    // Constructors
    Admin(int id) : adminID(id) {}

    // Copy Constructor
    Admin(const Admin &other) : Person(other), adminID(other.adminID) {}

    int get_Ad_id()
    {
        return adminID;
    }

    void set_ad_id(int i)
    {
        adminID = i;
    }

    void increaseCourseCapacity(Course &course, int additionalCapacity)
    {
        // Increase the course capacity by the specified amount
        course.courseCapacity += additionalCapacity;

        // Process students from the waitlist
        while (!course.Waitlist.empty() && course.courseCapacity > course.Waitlist.size())
        {
            int studentID = course.Waitlist.front();
            course.Waitlist.pop();
            cout << "Enrolled student from waitlist for course: " << course.courseCode << endl;
        }

        cout << "Course capacity increased successfully.\n";
    }

    void display() const override
    {
        cout << "Admin ID: " << adminID << ", Name: " << Name << ", PersonID: " << PersonID << endl;
    }
};

class EnrollmentSystem
{
public:
    vector<Course> CoursesVector;
    vector<Student> students;

    void enrollStudentInCourse(Student &s, Course &c)
    {
        s.enrollCourse(c);
    }

    void display() const
    {
        for (const auto &course : CoursesVector)
        {
            cout << "Course: " << course.courseCode << ", Capacity: " << course.courseCapacity
                 << ", Enrolled Students: " << course.courseCapacity - course.Waitlist.size() << endl;
        }
    }

    void displayStudents() const
    {
        cout << "Student Information:\n";
        cout << setw(15) << left << "StudentID" << setw(20) << left << "Student Name"
             << setw(20) << left << "Enrolled Courses"
             << "\n";

        for (const auto &student : students)
        {
            cout << setw(15) << left << student.studentID << setw(20) << left << student.studentName;

            // Display enrolled courses for each student
            if (!student.enrolledCourses.empty())
            {
                for (const auto &course : student.enrolledCourses)
                {
                    cout << course.courseCode << " ";
                }
            }
            else
            {
                cout << "None";
            }
            cout << "\n";
        }
    }

    void expelStudent(Student &student)
    {
        auto it = std::find_if(students.begin(), students.end(),
                               [&student](const Student &s)
                               { return s.studentID == student.studentID; });

        if (it != students.end())
        {
            students.erase(it);
            cout << "Student expelled successfully.\n";
            displayStudents();
        }
        else
        {
            cout << "Student not found in the list of students.\n";
        }
    }

    void addFromWaitlist(Course &c)
    {
        while (!c.Waitlist.empty() && c.courseCapacity > c.Waitlist.size())
        {
            int studentID = c.Waitlist.front();
            c.Waitlist.pop();
            cout << "Enrolled student from waitlist for course: " << c.courseCode << endl;
        }
    }

    template <typename T>
    void SearchCourseByInstructor(const T &instructor) const
    {
        cout << "Courses taught by " << instructor << ":" << endl;
        for (const auto &course : CoursesVector)
        {
            if (course.instructor == instructor)
            {
                cout << course << endl;
            }
        }
    }

    template <typename L>
    void SearchCourseByLevel(const L &level) const
    {
        cout << "Courses in level " << level << ":" << endl;
        for (const auto &course : CoursesVector)
        {
            if (course.level == level)
            {
                cout << course << endl;
            }
        }
    }

    template <typename D>
    void SearchCourseByDept(const D &department) const
    {
        cout << "Courses in department " << department << ":" << endl;
        for (const auto &course : CoursesVector)
        {
            if (course.department == department)
            {
                cout << course << endl;
            }
        }
    }

    void increaseCourseCapacity(Course &course, int additionalCapacity)
    {
        // Increase the course capacity by the specified amount
        course.courseCapacity += additionalCapacity;

        // Process students from the waitlist
        addFromWaitlist(course);

        // Find the course in CoursesVector and update its information
        auto it = find_if(CoursesVector.begin(), CoursesVector.end(),
                          [courseCode = course.courseCode](const Course &c)
                          { return c.courseCode == courseCode; });

        if (it != CoursesVector.end())
        {
            *it = course; // Update the course in CoursesVector
        }

        cout << "Course capacity increased successfully.\n";

        display();
    }

    

    // void saveEnrollmentToFile(const std::string &filename) const
    // {
    //     std::ofstream outFile(filename);

    //     outFile << "Enrollment Information:\n";
    //     outFile << setw(15) << left << "Course Code" << setw(20) << left << "Course Name"
    //             << setw(10) << left << "Credits" << setw(10) << left << "Capacity" << setw(15) << left << "Instructor"
    //             << setw(5) << left << "Level" << setw(10) << left << "Department"
    //             << "\n";

    //     for (const auto &course : CoursesVector)
    //     {
    //         outFile << setw(15) << left << course.courseCode << setw(20) << left << course.courseName
    //                 << setw(10) << left << course.courseCredits << setw(10) << left << course.courseCapacity
    //                 << setw(15) << left << course.instructor << setw(5) << left << course.level
    //                 << setw(10) << left << course.department << "\n";
    //     }

    //     // Save student information
    //     outFile << "\nStudent Information:\n";
    //     outFile << setw(10) << left << "StudentID" << setw(15) << left << "Student Name"
    //             << setw(20) << left << "Enrolled Courses" << setw(20) << left << "Dropped Courses"
    //             << "\n";

    //     for (const auto &student : students)
    //     {
    //         outFile << setw(10) << left << student.studentID << setw(15) << left << student.studentName;

    //         // Save enrolled courses for each student
    //         for (const auto &course : student.enrolledCourses)
    //         {
    //             outFile << course.courseCode << " ";
    //         }

    //         outFile << setw(20) << left; // Ensure correct spacing for the next column

            

    //         outFile << "\n";
    //     }

    // }


    void saveEnrollmentToFile(const std::string &filename) const
{
    std::ofstream outFile(filename);

    outFile << "Enrollment Information:\n";
    outFile << setw(15) << left << "Course Code" << setw(20) << left << "Course Name"
            << setw(10) << left << "Credits" << setw(10) << left << "Capacity" << setw(15) << left << "Instructor"
            << setw(5) << left << "Level" << setw(10) << left << "Department"
            << "\n";

    for (const auto &course : CoursesVector)
    {
        outFile << setw(15) << left << course.courseCode << setw(20) << left << course.courseName
                << setw(10) << left << course.courseCredits << setw(10) << left << course.courseCapacity
                << setw(15) << left << course.instructor << setw(5) << left << course.level
                << setw(10) << left << course.department << "\n";
    }

    // Save student information
    outFile << "\nStudent Information:\n";
    outFile << setw(10) << left << "StudentID" << setw(15) << left << "Student Name"
            << setw(20) << left << "Enrolled Courses" << setw(20) << left << "Dropped Courses"
            << "\n";

    for (const auto &student : students)
    {
        outFile << setw(10) << left << student.studentID << setw(15) << left << student.studentName;

        // Save enrolled courses for each student
        if (!student.enrolledCourses.empty())
        {
            for (const auto &course : student.enrolledCourses)
            {
                outFile << course.courseCode << " ";
            }
        }
        else
        {
            outFile << "None";
        }

        outFile << setw(20) << left; // Ensure correct spacing for the next column

        // Save dropped courses for each student
        if (!student.droppedCourses.empty())
        {
            for (const auto &course : student.droppedCourses)
            {
                outFile << course.courseCode << " ";
            }
        }
        else
        {
            outFile << "None";
        }

        outFile << "\n";
    }

    outFile.close();
}


        void loadEnrollmentFromFile(const std::string &filename)
        {
            std::ifstream inFile(filename);

            if (!inFile)
            {
                std::cerr << "Error opening file: " << filename << std::endl;
                return;
            }

            // Load course information
            CoursesVector.clear(); // Clear existing courses
            std::string courseCode, courseName, instructor, department;
            int courseCredits, courseCapacity, level;
            while (inFile >> courseCode >> courseName >> courseCredits >> courseCapacity >> instructor >> level >> department)
            {
                Course course(courseCode, courseName, courseCredits, courseCapacity, instructor, level, department);
                CoursesVector.push_back(course);
            }

            // Load student information
            students.clear(); // Clear existing students
            int studentID;
            std::string studentName;
            while (inFile >> studentID >> studentName)
            {
                Student student(studentID, studentName);

                // Load enrolled courses for each student
                std::string enrolledCourseCode;
                while (inFile >> enrolledCourseCode && enrolledCourseCode != "Dropped")
                {
                    auto it = std::find_if(CoursesVector.begin(), CoursesVector.end(),
                                           [enrolledCourseCode](const Course &c)
                                           { return c.courseCode == enrolledCourseCode; });
                    if (it != CoursesVector.end())
                    {
                        student.enrolledCourses.push_back(*it);
                    }
                }

                

                students.push_back(student);
            }

            inFile.close();
        }

        void removeCourseFromEnrollment(vector<Course> & CoursesVector, const Course &c)
        {
            auto it = find_if(CoursesVector.begin(), CoursesVector.end(),
                              [courseCode = c.courseCode](const Course &course)
                              { return course.courseCode == courseCode; });

            if (it != CoursesVector.end())
            {
                CoursesVector.erase(it);
                cout << "Course removed from enrollment successfully.\n";
            }
            else
            {
                cout << "Course not found in the list of enrolled courses.\n";
            }
        }
    };

    void studentMenu()
    {
        cout << "1. Enroll in a Course\n";
        cout << "2. Drop a Course\n";
        cout << "3. Generate Fee Challan\n";
        cout << "4. Search Course by Instructor\n";
        cout << "5. Search Course by Level\n";
        cout << "6. Search Course by Dept\n";
        cout << "0. Exit\n";
    }

    void adminMenu()
    {
        cout << "1. Add Course to Enrollment\n";
        cout << "2. Remove Course from Enrollment\n";
        cout << "3. Search Course by Instructor\n";
        cout << "4. Search Course by Level\n";
        cout << "5. Search Course by Dept\n";
        cout << "6. Expel Student\n";
        cout << "7. Increase Course Capacity\n";
        cout << "0. Exit\n";
    }

    int main()
    {
        EnrollmentSystem enrollmentSystem;
        enrollmentSystem.loadEnrollmentFromFile("enroll.txt");

        // Test Courses
        Course c1("BIO100", "Introduction to BIO", 10, 50, "Tariq", 100, "BIO");
        Course c2("CHEM300", "INTRO TO CHEMISTRY", 7, 40, "Basit", 300, "CHEM");

        Course c3("CS100", "COMPUTER SCIENCE", 5, 2, "JJ", 100, "CS");
        Course c4("PHY300", "MODERN PHYSICS", 8, 4, "BK", 300, "PHY");

        enrollmentSystem.CoursesVector.push_back(c1);
        enrollmentSystem.CoursesVector.push_back(c2);
        enrollmentSystem.CoursesVector.push_back(c3);
        enrollmentSystem.CoursesVector.push_back(c4);

        // Test Students
        Student s1(3, "Ali");
        Student s2(4, "Sarah");
        Student s3(1, "james");

        enrollmentSystem.students.push_back(s1);
        enrollmentSystem.students.push_back(s2);
        enrollmentSystem.students.push_back(s3);

        Admin admin(1);

        while (true)
        {
            string userType;
            cout << "Are you a Student or Admin? (Type 'student' or 'admin', 0 to exit): ";
            cin >> userType;

            if (userType == "0")
            {
                break;
            }

            if (userType == "student")
            {
                // Student Menu
                while (true)
                {
                    studentMenu();
                    int choice;
                    cout << "Enter your choice: ";
                    cin >> choice;
                    cin.ignore();

                    switch (choice)
                    {
                    case 1:
                        enrollmentSystem.enrollStudentInCourse(s2, c2);

                        break;
                    case 2:
                        s2.dropCourse();
                        break;
                    case 3:
                        s2.GenerateFee();
                        break;
                    case 4:
                        enrollmentSystem.SearchCourseByInstructor(c3.instructor);
                        break;
                    case 5:
                        enrollmentSystem.SearchCourseByLevel(c1.level);
                        break;
                    case 6:
                        enrollmentSystem.SearchCourseByDept(c2.department);
                        break;
                    case 0:
                        enrollmentSystem.saveEnrollmentToFile("enroll.txt");
                        break;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                    }

                    if (choice == 0)
                    {

                        enrollmentSystem.saveEnrollmentToFile("enroll.txt");
                        exit(0);
                    }

                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            else if (userType == "admin")
            {
                // Admin Menu
                while (true)
                {
                    adminMenu();
                    int choice;
                    cout << "Enter your choice: ";
                    cin >> choice;

                    switch (choice)
                    {
                    case 1:
                        enrollmentSystem.enrollStudentInCourse(s3, c1);
                        break;
                    case 2:
                        enrollmentSystem.removeCourseFromEnrollment(enrollmentSystem.CoursesVector, c1);
                        break;
                    case 3:
                        enrollmentSystem.SearchCourseByInstructor(c1.instructor);
                        break;
                    case 4:
                        enrollmentSystem.SearchCourseByLevel(c2.level);
                        break;
                    case 5:
                        enrollmentSystem.SearchCourseByDept(c2.department);
                        break;
                    case 6:
                        enrollmentSystem.displayStudents();
                        cout << endl;
                        enrollmentSystem.expelStudent(s1);
                        break;
                    case 7:

                        enrollmentSystem.display();
                        cout << endl;
                        enrollmentSystem.increaseCourseCapacity(c1, 10);
                        break;
                    case 0:
                        enrollmentSystem.saveEnrollmentToFile("enroll.txt");
                        break;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                    }

                    if (choice == 0)
                    {
                        enrollmentSystem.saveEnrollmentToFile("enroll.txt");
                        exit(0);
                    }

                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            else
            {
                cout << "Invalid user type. Please try again.\n";
            }
        }

        cin.get(); // Wait for user input before exiting
        return 0;
    }

