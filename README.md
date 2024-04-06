# Water Supply System

## Overview

Welcome to the Water Supply Management System! This system is designed to help users explore statistics about the Portuguese Water Supply System. It leverages real-world data about pipelines, pumping stations, delivery sites and water reservoirs to provide a comprehensive tool for informed decisions.

## Dataset

The system utilizes a dataset containing information about 23 cities, 175 pipelines, 25 water reservoirs and 82 pumping stations. This dataset includes details such as codes for each element, names, cities, demand, , and capacity, among other information.

## Features

### Data Handling
- Read and parse provided data, loading it into an appropriate graph data structure.

### Water Supply Management Analysis System Menu
- Develop a user-friendly system in order to expose the funcionalities developed.

## Basic Service Metrics

1. Calculate The Maximum Flow
   - Global Results
   - Specific City

2. List City Needs

3. Balance The System
   - Sorting of Differences Algorithm
   - Neighboring Pipes Algorithm
   - Average Capacity Algorithm

## Reliability and Sensitivity to Failures

1. List Cities Affected by Water Reservoir Removal
2. List Cities Affected by Pumping Station Removal
   - Global Results
   - Specific Pumping Station
3. List Cities Affected by Pipeline Removal
   - Global Results
   - Find Pipes That Would Affect a Specific City

### Documentation
- Include Doxygen documentation for relevant functions, indicating their time complexity.

## Implementation Details

This system is implemented in C++ using the provided `Graph` class for managing the network of the continental and Madeira water systems. The data is loaded from CSV files, and various functionalities are provided to assist users in navigating the flight network.


## Authors

Leonardo Garcia<br/>
Antonio Santos<br/>
Vanessa Queirós<br/>

### Thank you!
