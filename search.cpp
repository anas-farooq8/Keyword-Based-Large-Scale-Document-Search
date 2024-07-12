#include <iostream>             // For input/output operations
#include <fstream>              // For file handling
#include <sstream>              // For string streams
#include <vector>               // For dynamic arrays
#include <unordered_set>        // For hash-based sets (dictionary and stopwords)
#include <unordered_map>        // For hash-based maps
#include <algorithm>            // For sorting
#include <cmath>                // For mathematical functions
#include <iomanip>              // For formatted input/output
#include <cctype>               // For character classification
#include <chrono>               // For measuring time

using namespace std;
using namespace chrono;

/*
Using an unordered_set for both stopwords and dictionary words because it allows O(1) average time complexity
for lookups, insertions, and deletions.
*/
// Function to read stopwords and dictionary into sets
unordered_set<string> readWords(const string& filename, unordered_set<string>& words) {
    ifstream file(filename);

    string word;
    while (file >> word) {
        words.insert(word);
    }
    return words;
}

/*
Using a vector of pair<string, string> to store the documents. Each pair consists of the document ID and its content. 
*/
vector<pair<string, string>> readArticles(const string& filename, vector<pair<string, string>>& documents) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file " << filename << endl;
        return documents;
    }

    // Read the entire file content
    string fileContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    // Split content by Form Feed character (\x0C)
    stringstream ss(fileContent);
    string document;
    while (getline(ss, document, '\x0C')) {
        if (!document.empty()) {
            // Remove leading whitespace
            document.erase(document.begin(), find_if(document.begin(), document.end(), [](int ch) {
                return !isspace(ch);
            }));

            // Split the document into lines
            stringstream docStream(document);
            string docID;
            getline(docStream, docID); // The first line is the document ID

            string content;
            string line;
            while (getline(docStream, line)) {
                content += line + " ";
            }

            // Trim trailing whitespace from content
            if (!content.empty() && content.back() == ' ') {
                content.pop_back();
            }

            // Add the document ID and content to the vector if docID is not empty
            if (!docID.empty()) {
                documents.emplace_back(docID, content);
            }
        }
    }

    return documents;
}

/*
Using regular expressions to extract words and convert them to lowercase; then using the unordered_set
for dictionary and stopwords to filter the words.
*/
// Function to preprocess text: convert to lowercase, remove non-alphabetic characters, and remove stopwords and non-dictionary words
string preProcessText(const string& text, const unordered_set<string>& dictionary, const unordered_set<string>& stopwords) {
    string cleanedText;
    string word;
    for (char c : text) {
        if (isalpha(c)) {
            word += tolower(c);
        } else {
            if (!word.empty()) {
                if (dictionary.find(word) != dictionary.end() && stopwords.find(word) == stopwords.end()) {
                    cleanedText += word + " ";
                }
                word.clear();
            }
        }
    }

    // Process the last word
    if (!word.empty() && dictionary.find(word) != dictionary.end() && stopwords.find(word) == stopwords.end()) {
        cleanedText += word + " ";
    }

    return cleanedText;
}

// Function to calculate term frequency (TF) for each word in each
unordered_map<string, double> calculateTF(const string& text) {
    unordered_map<string, double> tf;
    stringstream ss(text);

    string word;
    int totalWords = 0;

    while (ss >> word) {
        tf[word]++;
        totalWords++;
    }

    // Calculate term frequency (TF)
    for (auto& pair : tf) {
        pair.second = (pair.second/totalWords) * 100;
    }
    
    return tf;
}

// Function to calculate inverse document frequency (IDF) for each word in the entire corpus
unordered_map<string, double> calculateIDF(const vector<pair<string, string>>& documents) {
    unordered_map<string, int> df;
    unordered_map<string, double> idf;
    int totalDocuments = documents.size();

    // Calculate document frequency (DF)
    for (const auto& doc : documents) {
        unordered_set<string> uniqueWords;
        stringstream ss(doc.second);
        string word;

        while (ss >> word) {
            uniqueWords.insert(word);
        }

        for (const auto& word : uniqueWords) {
            df[word]++;
        }
    }

    // Calculate inverse document frequency (IDF)
    for (const auto& pair : df) {
        idf[pair.first] = log10(static_cast<double>(totalDocuments) / pair.second);
    }

    return idf;
}

// Function to calculate the TF-IDF score for a document given the keywords
double calculateTFIDFScore(const unordered_map<string, double>& tf, const unordered_map<string, double>& idf, const vector<string>& keywords) {
    double score = 0.0;
    for (const auto& keyword : keywords) {
        if (tf.find(keyword) != tf.end() && idf.find(keyword) != idf.end()) {
            score += tf.at(keyword) * idf.at(keyword);
        }
    }
    return score;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " NUM keyword1 keyword2 ... keywordN" << endl;
        return 1;
    }

    int numResults = stoi(argv[1]);
    vector<string> keywords;
    for (int i = 2; i < argc; ++i) {
        keywords.push_back(argv[i]);
    }

    string dictionaryFile = "data/dictionary.txt";
    string stopwordsFile = "data/stopwords.txt";
    string articleFile = "data/article.txt";

    // Read dictionary and stopwords
    unordered_set<string> dictionary;
    readWords(dictionaryFile, dictionary);
    unordered_set<string> stopwords;
    readWords(stopwordsFile, stopwords);

    cout << "Dictionary contains " << dictionary.size() << " words." << endl;
    cout << "Stopwords contains " << stopwords.size() << " words." << endl;

    // Read and parse articles
    vector<pair<string, string>> documents;
    readArticles(articleFile, documents);
    cout << "Processed " << documents.size() << " documents." << endl;

    // Preprocess each document
    vector<pair<string, string>> preprocessedDocuments;
    for (const auto& doc : documents) {
        string cleanedContent = preProcessText(doc.second, dictionary, stopwords);
        preprocessedDocuments.emplace_back(doc.first, cleanedContent);
    }

    // The Main Algorithms stars from here
    auto start = high_resolution_clock::now(); // Start timing

    // Calculate TF for each document
    vector<unordered_map<string, double>> tfDocs;
    for (const auto& doc : preprocessedDocuments) {
        tfDocs.push_back(calculateTF(doc.second));
    }

    // Calculate IDF for the entire corpus
    unordered_map<string, double> idf = calculateIDF(preprocessedDocuments);

    // Calculate TF-IDF scores for each document
    vector<pair<double, pair<int, string>>> scores; // <score, <docIndex, docID>>
    for (int i = 0; i < preprocessedDocuments.size(); i++) {
        double score = calculateTFIDFScore(tfDocs[i], idf, keywords);
        if (score > 0) {
            scores.emplace_back(score, make_pair(i + 1, preprocessedDocuments[i].first));
        }
    }

    // Sort documents by their TF-IDF scores in descending order
    // Note: If the Sim value is the same, sort the web pages in ascending order by web page number(docId).
    stable_sort(scores.begin(), scores.end(), [](const pair<double, pair<int, string>>& a, const pair<double, pair<int, string>>& b) {
        return a.first == b.first ? a.second < b.second : a.first > b.first;
    });

    // Output the top 5 results to the screen
    cout << endl << "Top 5 results:" << endl;
    for (int i = 0; i < min(5, static_cast<int>(scores.size())); i++) {
        cout << fixed << setprecision(6) << scores[i].first << " " << scores[i].second.first << " " << scores[i].second.second << endl;
    }

    auto end = high_resolution_clock::now(); // End timing
    auto duration = duration_cast<milliseconds>(end - start).count(); // Calculate the elapsed time in milliseconds

    cout << "Time taken: " << duration << " milliseconds" << endl; // Output the elapsed time

    // Output the top N results to the results.txt file
    ofstream resultFile("results.txt");
    for (int i = 0; i < min(numResults, static_cast<int>(scores.size())); i++) {
        resultFile << fixed << setprecision(6) << scores[i].first << " " << scores[i].second.first << " " << scores[i].second.second << endl;
    }
    resultFile.close();
    
    return 0;
}
