
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
using namespace std;

const int NMISP = 5;
const int RNG_RANGE = 100;


/* This function is used to output the first output of the program */
void
display_header(const int ndict, const int nmiss)
{
    cout << "*** Typing Tutor" << endl;
    cout << "There are " << ndict << " words in the dictionary." << endl;
    cout << "There are " << nmiss << " commonly misspelled words." << endl;

}

void
display_menu_options()
{

    cout << endl << "*** Select a menu option:" << endl
         << "1. Spell commonly misspelled words" << endl
         << "2. Type random words from the full dictionary" << endl
         << "3. Display some words from one of the lists" << endl
         << "4. Use binary search to lookup a word in the dictionary" << endl
         << "5. Exit the program." << endl
         << "Your choice: ";
}


/* This function is used to return a vector, populated with 'n' number of random numbers(not greater than max) */
vector<int>
randn(const int n, const int max)
{
    vector<int> temp;

    for (int i=0; i<n; i++)                                                              // for loop runs 'n' times, and is used to populate the 'temp' vector with random values belonging to the range (0,max).
    {
        temp.push_back(rand()%(max+1));
    }
    return temp;
}

ifstream
open(const string& src_file)
{
    ifstream src{src_file};
    if (!src.is_open()) {
        cout << "Could not find file " << src_file << "." << endl;
        exit(1);
    }
    return src;
}


/* This function populates a vector 'dst' with content extracted from 'src_file'. */
void
extract_dict(vector<string>& dst, const string& src_file)
{

    ifstream myfile(src_file);
    string line;

    while(getline(myfile,line))
    {
        dst.push_back(line);
    }
}


/* This function takes in the name of a file as the parameter 'src_file' and then populates vectors 'misspelled' & 'correct' with contents of the source file. */
void
extract_misspelled(vector<string>& misspelled, vector<string>& correct,
                   const string& src_file)
{

    vector<string> temporary;                                                              // 'temporary' vector is being declared, and will be used to temporarily hold the values from 'src_file'.

    extract_dict(temporary, src_file);

    for (int i=0; i<temporary.size(); i++){
        string misspelled_ele;
        string correct_ele;
        int count = 0;

        for (int j=0; j<temporary[i].length(); j++){                                         // for loop pulls each element from temporary vector, and sorts them into two seperate variables(string data type).
            if (count == 0){
                if(temporary[i][j] == ' '){
                    count++;
                }

                else{
                    misspelled_ele += temporary[i][j];                                             // misspelled_ele is later pushed back into misspelled vector.
                }
            }

            else{
                correct_ele += temporary[i][j];                                                  // correct_ele is later pushed back into misspelled vector.
            }
        }

        correct.push_back(correct_ele);
        misspelled.push_back(misspelled_ele);

    }
}

/* This function outputs elements (on specific positions) of the vector 'src'. */
void
display_elems(const vector<string>& src, const vector<int>& positions)
{

    for (int i=0; i<(positions.size()); i++){
        if (i != positions.size()-1){
            cout << src.at(positions.at(i)) << " ";
        }

        else{
            cout << src.at(positions.at(i));
        }
    }
}


/* This function randomly generates correct answers, which are placed in the vector 'dst' */
void
extract_spelling_answers(vector<string>& dst, const vector<string>& correct,
                         const vector<int> positions)
{

    for (int i=0; i<positions.size(); i++){
        dst.push_back(correct.at(positions.at(i)));
    }
}


/*
This function:
      1. seperates user's answers from the string 'to_grade'.
      2. pushes wrong answers to an empty vector 'dst'.
*/
void
extract_spelling_mistakes(vector<string>& dst, const string& to_grade,
                          const vector<string>& correct)
{

    string x;
    string new_str="";
    new_str = to_grade + " ";
    int i = new_str.find(' ');
    vector<string> temp;

    while (i<(new_str.size())){
        x = new_str.substr(0, i);
        temp.push_back(x);
        new_str = new_str.substr(i+1, new_str.size()-i);
        i = new_str.find(' ');
    }

    for (int j=0; j<temp.size(); j++){
        if (temp.at(j) != correct.at(j)){
            dst.push_back(temp.at(j) + " -> " + correct.at(j));
        }
    }
}


/* This function checks for the user's total score, and outputs them. */
void
report_misspell_mistakes(const vector<string>& mistakes,
                         const int num_words)
{

    int correct_count = num_words - mistakes.size();

    if (mistakes.size() == 0){
        cout << "No misspelled words!" << endl;
    }

    else{
        cout << "Misspelled words:" << endl;
        for (int i=0; i<mistakes.size(); i++){
            cout << "    " << mistakes.at(i) << endl;
        }
    }

    int score = 0;
    score = (correct_count * 3) - ((mistakes.size()) * 6);

    cout << correct_count * 3 << " points: " << correct_count << " spelled correctly" << " x 3 points for each." << endl;
    cout << (mistakes.size()) * 6 << " point penalty: " << mistakes.size() << " spelled incorrectly" << " x -6 points for each." << endl;
    cout << "Score: " << score << endl;
}


/*
This function gets the user's input, corresponding to the random words picked beforehand.
And, calls aforementined functions:
    1. extract_spelling_answers
    2. extract_spelling_mistakes
    3. report_misspell_mistakes
*/
void
evaluate_user_misspells(const vector<string>& src, const vector<int>& positions)
{

    string input;
    getline(cin, input);

    vector<string> mistakes;
    vector<string> answers;

    extract_spelling_answers(answers, src, positions);

    extract_spelling_mistakes(mistakes, input, answers);

    report_misspell_mistakes(mistakes, NMISP);
}


/*
This function is to put together the first option of the menu. And, it does so by calling:

    1. display_elems
    2. evaluate_user_misspells
*/
void
misspelled(const vector<string>& mspelled, const vector<string>& correct)
{

    vector<int> positions = randn(NMISP, correct.size()-1);

    cout << endl;
    cout << "*** Using commonly misspelled words." << endl;

    display_elems(mspelled, positions);

    cout << endl;
    cout << "Type the words above with the correct spelling: " << endl;

    evaluate_user_misspells(correct, positions);
}


/* This function is used to drive the 3rd option of the menu and ask the spelling of a random word from the whole of the dictionary. */
void
full_dict(const vector<string>& dict)
{

    vector<int> positions = randn(NMISP, dict.size()-1);

    cout << endl;
    cout << "*** Using full dictionary." << endl;

    display_elems(dict, positions);

    cout << endl;
    cout << "Correctly type the words above: " << endl;
    evaluate_user_misspells(dict, positions);
}

void
display_words_prompt()
{
    // This displays the data set choices for menu option 3
    cout << endl << "*** Displaying word list entries." << endl
         << "Which words do you want to display?" << endl
         << "A. Dictionary of all words" << endl
         << "B. Commonly misspelled words (wrongly spelled)" << endl
         << "C. Commonly misspelled words (correctly spelled)" << endl
         << "Your choice: ";
}


/* Outputs the contents of a vector 'data' in the range provided by the user. */
void
print_range(const vector<string>& data)
{

    cout << " Enter the start and end indices between 0 and " << (data.size()-1) << ": ";
    int start = 0;
    int end = 0;

    cin >> start >> end;

    for (int i=start; i<=end; i++){
        cout << i << ". " << data.at(i) << endl;
    }
}


/* This function can be used buy the user to output the contents(in the given range) of the selected vector. */
void
display_in_range(const vector<string>& dict, const vector<string>& mspelled,
                 const vector<string>& correct)
{

    cout << "\n*** Displaying word list entries." << endl;
    cout << "Which words do you want to display?" << endl;
    cout << "A. Dictionary of all words" << endl;
    cout << "B. Commonly misspelled words (wrongly spelled)" << endl;
    cout << "C. Commonly misspelled words (correctly spelled)" << endl;
    cout << "Your choice: ***";

    char choice;
    cin >> choice;
    choice = toupper(choice);

    switch (choice){
        case 'A':
            print_range(dict);
            break;

        case 'B':
            print_range(mspelled);
            break;

        case 'C':
            print_range(correct);
            break;}
}


/* This function drives the 4th option of the program, wherein the user is able to find the provided word using binary search algorithm. */
void
bsearch(const vector<string>& data)
{
    string sWord;                                                                          // target
    cout << "*** Enter word to search: ";
    cin >> sWord;
    int high = data.size();                                                                // upper limit, which gets updates every time.
    int low = 0;
    int count = 1;                                                                         // this variable gets incremented every time the while loop runs.
    int find = 0;

    while(high > low){

        int mid = ((high + low + 2) / 2) - 1;

        if (count > 9){
            cout << "   " << count << ". Comparing to: " << data.at(mid) << endl;
        }

        else{
            cout << "    " << count << ". Comparing to: " << data.at(mid) << endl;
        }

        if (data.at(mid) > sWord){                                                           // if the target word lies to the left of the middle element, the variable 'high' takes the value of mid

            high = mid;
        }

        else if(sWord > data.at(mid)){                                                       // if the target word lies to the right of the middle element, the variable 'low' takes the value of mid+1

            low = mid + 1;
        }

        else{                                                                                // else loop runs in case the target word is found.

            cout << sWord << " was found.\n";
            return;                                                                            // returns void, so that the funciton stops executing
        }

        count++;
    }

    cout << sWord << " was NOT found.";                                                    // outputs the statement, only if the target word is not found.

}

void
extract_all_data(vector<string>& dict, vector<string>& mspelled,
                 vector<string>& correct)
{
    extract_dict(dict, "dictionary.txt");
    extract_misspelled(mspelled, correct, "misspelled.txt");
}

int
main()
{
    srand(1);
    vector<string> dict, mspelled, correct;
    extract_all_data(dict, mspelled, correct);
    display_header(dict.size(), correct.size());
    unsigned int choice;
    do {
        display_menu_options();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch (choice) {
            case 1:
                misspelled(mspelled, correct);
                break;
            case 2:
                full_dict(dict);
                break;
            case 3:
                display_in_range(dict, mspelled, correct);
                break;
            case 4:
                bsearch(dict);
                break;
            case 5:
                cout << "Exiting." << endl;
                break;
            default:
                cout << "No menu option " << choice << ". "
                     << "Please try again." << endl;
        }
    } while ((choice != 5) && !cin.eof());



    return 0;
}