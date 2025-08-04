# ⚡ Run Tests in an Algorithm Sandbox

## 📌 Project Overview

This project demonstrates how to build a **safe, controlled test environment** for running and comparing **sorting algorithms** on a microcontroller such as the **Arduino Uno**.
It measures **execution time**, checks **correctness**, and can be extended to log results to **SD card** for later analysis.

---

## 🎯 Goal

* Learn to implement and compare multiple sorting algorithms on limited hardware.
* Practice using **sandbox-style testing** to ensure algorithms run safely.
* Measure **performance** and **correctness** on small datasets.

---

## 🛠 Features

* Implements **Bubble Sort**, **Quicksort**, and **Mergesort**.
* **Measures execution time** using `micros()` (microseconds).
* **Checks correctness** of the sorted output.
* Dataset generated randomly on each run.
* Designed for **low-memory environments** like the Arduino Uno.

---

## 🔌 Hardware Requirements

| Component                 | Notes                              |
| ------------------------- | ---------------------------------- |
| Arduino Uno               | Or compatible ATmega328P board     |
| USB cable                 | For programming and Serial Monitor |
| (Optional) SD card module | For storing results                |

---

## 📡 How It Works

1. **Generate** a random dataset of integers.
2. **Run** each sorting algorithm on a copy of the dataset.
3. **Measure** execution time with `micros()`.
4. **Verify** sorted output.
5. **Display** results via the Serial Monitor.

---

## 📋 Example Output (Serial Monitor)

```
=== Sorting Algorithm Performance Test ===
BubbleSort | Time: 2450 us | Correct: Yes
QuickSort  | Time: 600 us | Correct: Yes
MergeSort  | Time: 700 us | Correct: Yes
```

---

## 💻 Code

```cpp
// =====================================
// Sorting Algorithm Performance Tester
// For Arduino Uno
// =====================================

#define DATASET_SIZE 30  // Keep small for Uno

int dataset[DATASET_SIZE];
int tempArray[DATASET_SIZE]; // For mergesort

// ------------------------------
// Utility: Generate random dataset
// ------------------------------
void generateDataset() {
  for (int i = 0; i < DATASET_SIZE; i++) {
    dataset[i] = random(0, 1000);
  }
}

// ------------------------------
// Utility: Check if array sorted
// ------------------------------
bool isSorted(int arr[], int n) {
  for (int i = 1; i < n; i++) {
    if (arr[i] < arr[i - 1]) return false;
  }
  return true;
}

// ------------------------------
// Bubble Sort
// ------------------------------
void bubbleSort(int arr[], int n) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        int temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}

// ------------------------------
// Quicksort
// ------------------------------
int partitionQS(int arr[], int low, int high) {
  int pivot = arr[high];
  int i = low - 1;
  for (int j = low; j < high; j++) {
    if (arr[j] <= pivot) {
      i++;
      int temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;
    }
  }
  int temp = arr[i + 1]; arr[i + 1] = arr[high]; arr[high] = temp;
  return i + 1;
}

void quickSort(int arr[], int low, int high) {
  if (low < high) {
    int pi = partitionQS(arr, low, high);
    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
  }
}

// ------------------------------
// Mergesort
// ------------------------------
void merge(int arr[], int l, int m, int r) {
  int i = l, j = m + 1, k = l;

  while (i <= m && j <= r) {
    if (arr[i] <= arr[j]) tempArray[k++] = arr[i++];
    else tempArray[k++] = arr[j++];
  }
  while (i <= m) tempArray[k++] = arr[i++];
  while (j <= r) tempArray[k++] = arr[j++];

  for (i = l; i <= r; i++) arr[i] = tempArray[i];
}

void mergeSort(int arr[], int l, int r) {
  if (l < r) {
    int m = (l + r) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
  }
}

// ------------------------------
// Test and time algorithms
// ------------------------------
void testAlgorithm(void (*sortFunc)(int*, int), const char* name) {
  int arr[DATASET_SIZE];
  memcpy(arr, dataset, sizeof(dataset)); // Copy dataset

  unsigned long startTime = micros();
  sortFunc(arr, DATASET_SIZE);
  unsigned long duration = micros() - startTime;

  Serial.print(name);
  Serial.print(" | Time: ");
  Serial.print(duration);
  Serial.print(" us | Correct: ");
  Serial.println(isSorted(arr, DATASET_SIZE) ? "Yes" : "No");
}

void testQuickSort() {
  int arr[DATASET_SIZE];
  memcpy(arr, dataset, sizeof(dataset));

  unsigned long startTime = micros();
  quickSort(arr, 0, DATASET_SIZE - 1);
  unsigned long duration = micros() - startTime;

  Serial.print("QuickSort | Time: ");
  Serial.print(duration);
  Serial.print(" us | Correct: ");
  Serial.println(isSorted(arr, DATASET_SIZE) ? "Yes" : "No");
}

void testMergeSort() {
  int arr[DATASET_SIZE];
  memcpy(arr, dataset, sizeof(dataset));

  unsigned long startTime = micros();
  mergeSort(arr, 0, DATASET_SIZE - 1);
  unsigned long duration = micros() - startTime;

  Serial.print("MergeSort | Time: ");
  Serial.print(duration);
  Serial.print(" us | Correct: ");
  Serial.println(isSorted(arr, DATASET_SIZE) ? "Yes" : "No");
}

// ------------------------------
// Setup
// ------------------------------
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));

  generateDataset();
  Serial.println("=== Sorting Algorithm Performance Test ===");

  testAlgorithm(bubbleSort, "BubbleSort");
  testQuickSort();
  testMergeSort();
}

void loop() {
  // No loop actions
}
```

---

## 🚀 Possible Improvements

* Run **multiple trials** and compute average times.
* Store results on an **SD card**.
* Add **dataset size scaling** to see how performance changes.
* Track **approximate memory usage** for each algorithm.


