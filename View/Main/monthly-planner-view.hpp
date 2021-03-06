#include <stdexcept>
#include <vector>
#include <iomanip>
#include <iostream>

#include "../../Controller/util.hpp"
#include "../../Model/monthly-planner/monthly-planner.hpp"

using namespace std;

namespace view {
    bool mpMenu ();
    int askMPMenu ();
    void addMP ();
    void displayMP ();
    void checkForMissedMP ();
}

#ifndef __MONTHLYVH__
#define __MONTHLYVH__

inline bool view::mpMenu ()
{
    int option = view::askMPMenu();
    if (option == 1)
    {
        view::displayMP();
    }
    else if (option == 2)
    {
        view::addMP();
    }
    else if (option == 4)
    {
        return true;
    }
    return false;
}

inline int view::askMPMenu ()
{
    int option=0;
    while (true)
    {
        cout << "\n1. Display all Plans:\n";
        cout << "2. Add new Plan\n";
        cout << "3. Main Menu\n";
        cout << "4. Exit\n";
        cout << "Choose a option: ";
        cin >> option;
        if (clamp(option, 1, 4))
        {
            break;
        }
        else
        {
            cout << "\nPlease choose an option amongst the following: ";
        }
    }
    return option;
}

inline void view::addMP ()
{
    std::string planner;
    cout << "Enter the planner objective: ";
    std::ws(cin);
    std::getline(cin, planner);
    try
    {
        MonthlyPlanner newPlan = MonthlyPlanner(planner);
        newPlan.save();
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
    }
    cout << "New plan created!!\n";
}

inline void view::displayMP ()
{
    char option;
    cout << "\nView completed plans? (y/n)";
    cin >> option;
    bool getCompleted {option == 'y' || option == 'Y'};
    std::vector<MonthlyPlanner> plans{MonthlyPlanner::getallPlans(getCompleted)};
    if (plans.size() == 0)
    {
        cout << "No plans to display...\n";
        return;
    }

    cout << "S.No.\tPlan";
    cout << setw(60);
    cout << "Created At";
    cout << "\n";
    for (int i=0; i<plans.size(); i++)
    {
        cout << i+1 << "\t";
        cout << plans[i].monthlyPlan;
        cout << setw(60);
        cout << plans[i].createdAt;
        cout << "\n";
    }
    if (getCompleted)
    {
        return;
    }
    cout << "\nEnter plan number to complete it (0 to ignore): ";
    int planNum;
    cin >> planNum;
    try
    {
        if (clamp(planNum, 1, plans.size()))
        {
            plans[planNum-1].completePlan();
        }
    }
    catch(const std::runtime_error& e)
    {
        cout << e.what() << '\n';
    }
}

inline void view::checkForMissedMP ()
{
    try
    {
        MonthlyPlanner::checkAnRemoveExpiredPlan();
    }
    catch(const std::runtime_error& e)
    {
        std::cout << e.what() << "\n";
    }
    
}

#endif