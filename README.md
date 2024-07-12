# Keyword-Based Large-Scale Document Search

This project implements a keyword-based document search engine to efficiently search and sort large-scale text documents. The assignment includes performance testing with large amounts of test data, assessing the mastery of data structures and algorithms in C++.

## Table of Contents
- Project Description
- Features
- Installation
- Usage
- File Descriptions
- Implementation Details
- Demo


## Project Description
The goal of this project is to implement a document search program based on keywords to achieve quick searching and sorting of large-scale text documents. It involves extracting words from documents, counting their frequency, and calculating term frequency (TF) and inverse document frequency (IDF) for relevance scoring. The implementation covers:
- Efficiently extracting and processing words from a large set of documents.
- Calculating term frequency (TF) and inverse document frequency (IDF).
- Scoring documents based on TF-IDF and sorting them by relevance.
- Handling both small and large data sets.
- Outputting the top search results to both the screen and a file.

## Features
- Handles both small and large data sets efficiently.
- Uses dictionary and stopword files for accurate word frequency calculation.
- Outputs top results to both the screen and a file.
- Performance testing to ensure efficient searching.
- Supports multiple keywords for searching.

## Installation
1. Clone the repository:
    ```sh
    git clone https://github.com/anas-farooq8/Keyword-Based-Large-Scale-Document-Search.git
    cd Keyword-Based-Large-Scale-Document-Search
    ```

2. Ensure you have a C++ compiler installed (e.g., g++, clang).

3. Place the required files (`dictionary.txt`, `stopword.txt`, `article.txt`) in the data directory in the root folder.

## Usage
### Compiling the Program
Compile the program using a C++ compiler:
```sh
g++ -o search search.cpp
```

### Running the Program
```sh
./search NUM K1 K2 ...Km

Example:
./search 100 edu news article
```

### Input Format
* The number of search results (NUM).
* The search keywords (K1 K2 ...Km).

### Output Format
* The program outputs the top N results ranked by relevance to the screen.
* The results are also saved to a file named results.txt.

## File Descriptions
* search.cpp: Main program file containing the implementation of the search engine.
* dictionary.txt: File containing dictionary words.
* stopword.txt: File containing stopwords.
* article.txt: File containing the web page documents.
* results.txt: Output file containing the top N search results.
* results(example).txt: Sample output file for reference.

## Implementation Details
### Data Processing
* Extract Words: Extract words from the documents in article.txt, convert them to lowercase, and filter out stopwords.
* Count Frequencies: Count the frequency of each word in each document.
* Calculate TF and IDF: Compute the term frequency (TF) and inverse document frequency (IDF) for each word.
* Score and Sort: Use TF-IDF to score the relevance of documents based on the input keywords and sort the results.

### Performance Optimization
* Utilize efficient data structures (e.g., hash maps) for fast lookups and frequency counting.
* Optimize the TF-IDF calculation to handle large data sets within reasonable time limits.


## Demo
![Screenshot (87)](https://github.com/user-attachments/assets/538d4ce9-9376-4dc2-bce8-b11c0ba7e7f2)
![Screenshot (88)](https://github.com/user-attachments/assets/e820e92e-2ac3-40b6-8e3e-79889081db5e)
