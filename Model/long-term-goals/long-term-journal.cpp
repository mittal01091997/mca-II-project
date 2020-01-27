#ifndef __LTJH__
    #define __LTJH__
    #include "./long-term-journal.h"
#endif

#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

#ifndef _CHRONO_
    #define _CHRONO_
    #include <chrono>
#endif

#ifndef _FSTREAM_
    #define _FSTREAM_
    #include <fstream>
#endif

#ifndef _STDEXCEPT_
    #define _STDEXCEPT_
    #include <stdexcept>
#endif

#ifndef _VECTOR_
    #define _VECTOR_
    #include <vector>
#endif

#ifndef _STRING_
    #define _STRING_
    #include <string>
#endif

std::string ltgjournals = std::string("ltgjournal.dat");

LongTermGoalJournal::LongTermGoalJournal () noexcept
{
    this->longTermGoalId = 0;
}

LongTermGoalJournal::LongTermGoalJournal (std::string journal, unsigned int ltgId) noexcept
{    
    // adding a delimiter to take multiword input with grace
    this->journal = (journal + std::string("$"));

    auto now = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
    using sys_seconds = decltype(now);
    this->lodgeDate = now.time_since_epoch().count();

    this->longTermGoalId = ltgId;

    /*
    to convert time back fron (long) duration to time_point 

    1)
    std::chrono::time_point<std::chrono::system_clock> dt(std::chrono::seconds(duration));
    dt is now the value we need to use

    2)
    auto now = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
    using sys_seconds = decltype(now);
    sys_seconds dt{std::chrono::seconds{duration}};
    */
}

// TODO: take input number of journals logded, and delete excess journals
// keep only 10
void LongTermGoalJournal::save (unsigned int journalsLodged=0) const noexcept(false)
{
    std::ofstream wfileptr;
    wfileptr.open(ltgjournals, std::ios::out);
    if (!wfileptr.good())
    {
        throw std::runtime_error("Couldnot save the journal for the Goal!!");
    }
    wfileptr.write((char *)this, sizeof(this));
    wfileptr.close();

    // if (journalsLodged > 10)
    // {
    //     // delete a journal
    //     std::ifstream rfileptr;
    //     bool recordToDeleteMissed = false;

    //     rfileptr.open(ltgjournals, std::ios::in);
    //     if (!rfileptr.good())
    //     {
    //         throw std::runtime_error("Unable to process deletion of extra journals");
    //     }

    //     wfileptr.open("temp.dat",std::ios::out);
    //     if (!wfileptr.good())
    //     {
    //         throw std::runtime_error("Unable to process deletion of extra journals");
    //     }

    //     LongTermGoalJournal ltgJournal;
    //     while (rfileptr.read((char *)&ltgJournal, sizeof(ltgJournal)))
    //     {
    //         if (ltgJournal.longTermGoalId == goalId)
    //         {
    //             if (!recordToDeleteMissed)
    //             {
    //                 recordToDeleteMissed = true;
    //             }
    //             else
    //             {
    //                 wfileptr.write((char *)&ltgJournal, sizeof(ltgJournal));
    //             }
                
    //         }
    //     }

    //     wfileptr.close();
    //     rfileptr.close();

    //     remove(ltgjournals);
    //     rename("temp.dat", ltgjournals);
    // }
}

std::vector<LongTermGoalJournal> LongTermGoalJournal::getJournals (unsigned int goalId) noexcept(false)
{
    std::vector<LongTermGoalJournal> journals;
    
    std::ifstream fileptr;
    fileptr.open(ltgjournals, std::ios::in);
    if (!fileptr.good())
    {
        throw std::runtime_error("Couldn't get the journals to display!!");
    }

    LongTermGoalJournal obj;

    while (!fileptr.eof())
    {
        std::getline(fileptr, obj.journal, '$');
        fileptr >> obj.lodgeDate;
        fileptr >> obj.longTermGoalId;

        if (obj.longTermGoalId == goalId)
        {
            journals.push_back(obj);
        }
    }

    fileptr.close();
    return journals;
}

std::ifstream& LongTermGoalJournal::operator >> (std::ifstream& stream)
{
    stream.read((char *)this, sizeof(this));
    return stream;
}

std::ofstream& LongTermGoalJournal::operator<<(std::ofstream& stream)
{
    stream.write((char *)this, sizeof(this));
    return stream;
}