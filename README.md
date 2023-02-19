# My-Projects
Functionality：
This program has realized functionalities of getting orders from customers, and then the program calculates cooking time of each ordered food according to the following rules. The .exe and input file are under EXE directory. User can run FishAndChips.exe and will get output file. User can add new orders in Input.txt file according to the following explanation, and will get the corresponding different output.

** Rules **
A restaurant sells fish and chips, and customers can order cod, haddock and chips. 
-The cooking time span of one portion of cod is 80 seconds.
-The cooking time span of one portion of haddock is 90 seconds.
-The cooking time span of one portion of chips is 120 seconds.
-4 portions of chips can be cooked at a time.
-There are 4 individual hobs for cooking fish. Each hob can cook one portion of cod or haddock at a time.
-The food has be served to customers as quickly as possible after ordering. 
-The time span from ordering to serving has to less than 600 seconds.
-The cooked food has to be served within 120 seconds. The restaurant would like to serve freshly cooked food.

** Explanation of input file: **
-For example: Order #1, 12:00:00, 2 Cod, 4 Haddock, 3 Chips.
-Explanation: At 12:00:00, the restaurant gets the first order. A customer orders 2 portions of cod, 4 portions of haddock and 3 portions of chips.

** Software skills in this program: **
-Using STL of <vector>, <stack>, <algorithm>.
-Using polymorphism: in OutputInstructions.h and OutputInstructions.cpp files, both class OutputInstructionsToFile and class OutputInstructionsToScreen inherit from class BaseOutput. WriteAcceptedOrder() and WriteRejectedOrder() are two pure virtual functions.
-.txt file creation, open, close, read and write.

** Source Code Explanation **
-FishAndChips.h/FishAndChips.cpp: include main entry function and main process of this application.
-ParseOrder.h/ParseOrder.cpp: read orders from input file/screen, and parse each order to OrderInfo structure.
-CreateInstructions.h/CreateInstructions.cpp: create cooking instructions and order status of current order.
-OutputInstructions.h/OutputInstructions.cpp: output cooking instructions and order status to .txt file.
-global.h: includes head files and define structures.


