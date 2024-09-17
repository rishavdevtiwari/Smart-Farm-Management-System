//Smart Farm Management System
#include <stdio.h>//This line tells the compiler to include the 'stdio.h' header file, which stands for "standard input/output".
#include <stdlib.h>//This line includes the 'stdlib.h' header file, which stands for "standard library".
#include <string.h>//This line includes the 'string.h' header file, which contains functions for manipulating strings.

#define MAX_DATE_LENGTH 11//Define the length of date in format "DD/MM/YYY"
#define COLOR_RED "\x1B[31m"//Define the color code for output in printf as the colour red.
#define COLOR_BLUE "\x1b[34m"//Define the color code for output in printf as the color blue.
#define COLOR_GREEN "\x1B[32m"//Define the color code for output in printf as the color green.
#define COLOR_YELLOW "\x1b[33m"//Define the color code for output in printf as the color green.
#define COLOR_RESET "\x1B[0m"//Define the reset color code for output in printf.

// Define a structure to hold crop-related information
typedef struct {
    char name[30];// Name of the crop (maximum length of 30 characters) Dynamically allocated
    float area;// Area of the crop in acres (floating-point number)
    float yield;// Yield of the crop in tons (floating-point number)
    char plantingDate[MAX_DATE_LENGTH];     // Date when the crop was planted (maximum length of 10 characters)//+
                                           // The date is represented as a string in the format "MM/DD/YYYY"/
    char harvestDate[MAX_DATE_LENGTH];    // Date when the crop is expected to be harvested (maximum length of 10 characters)//+
                                         // The date is represented as a string in the format "MM/DD/YYYY"/
    int status;// Status of the crop 1: Planted 2: Ready to Harvest 3: Harvested
} Crop;

// Define a structure to hold field-related information
typedef struct {
    char type[30];//Type of field (maximum length of 30 characters) Dynamically allocated
    float area; //Area of field in acres (floating-point number)
    float soilMoisture;//Soil moisture in percentage (can be updated)
} Field;

//Define a structure to hold Expense-related information
typedef struct {
    char name[20];//Name of expense(maximum length of 20 characters)
    char category[30];//Name of category(maximum length of 30 characters)
    float amount;//Expense amount(floating-point number)
} Expense;

// Define farm data structure
typedef struct {
    Crop* crops;//crops is a pointer to an array of Crop objects, which represents the different types of crops grown on the farm.
    int cropCount;//cropCount is an integer that keeps track of the number of crops currently stored in the crops array.
    int cropCapacity;//cropCapacity is an integer that represents the maximum number of crops that the crops array can hold.

    Field* fields;//fields is a pointer to an array of Field objects, which represents the different fields on the farm.
    int fieldCount;//fieldCount is an integer that keeps track of the number of fields currently stored in the fields array.
    int fieldCapacity;//fieldCapacity is an integer that represents the maximum number of fields that the fields array can hold.

    Expense* expenses;//expenses is a pointer to an array of Expense objects, which represents the different expenses incurred by the farm.
    int expenseCount;//expenseCount is an integer that keeps track of the number of expenses currently stored in the expenses array.
    int expenseCapacity;//expenseCapacity is an integer that represents the maximum number of expenses that the expenses array can hold.
} FarmData;

// Defining all required functions for the program
void showMainMenu();//Display the main menu

void manageCrops(FarmData *data);//Manage crops on the farm
void addCrop(FarmData *data);//Add a new crop
void viewCrops(FarmData *data);//Display all crops
void displayCropStatuses(FarmData *data);//Show crop statuses
void updateCropStatus(FarmData *data);//Update a crop's status
void deleteCrop(FarmData *data);//Remove a crop entry

void irrigationScheduling(FarmData *data);//Schedule irrigation for fields
void addField(FarmData* data);//Add a new field
void displayFieldDetails(FarmData* data);//Show field details
void calculateWaterNeeded(FarmData* data);//Calculate water needed for fields
void updateSoilMoisture(FarmData* data);//Update soil moisture levels
void deleteField(FarmData* data);//Remove a field entry

void expenseTracking(FarmData *data);//Manage farm expenses
void addExpense(FarmData *data);//Add a new expense
void viewExpenses(FarmData *data);//Display all expenses
void calculateTotalAndAverageExpenses(FarmData *data);//Calculate total and average expenses
void deleteExpense(FarmData *data);//Remove an expense

void saveCropsToFile(FarmData *data);//save crops to a file
void saveFieldsToFile(FarmData *data);//save fields to a file
void saveExpensesToFile(FarmData *data);//save expenses to a file
void loadCropsFromFile(FarmData *data);//load crops from a file
void loadFieldsFromFile(FarmData *data);//load fields from a file
void loadExpensesFromFile(FarmData *data);//load expenses from a file

void clearInputBuffer();//clear input buffer && handle string errors
void freeData(FarmData *data);//Free farm data memory for optimization of memory
int validateDate(const char *date);//validate a date string in format DD/MM/YYYY

// Main function
int main() {
    FarmData data; //FarmData structure holds crop,field and expense data
    data.cropCount = 0;
    data.cropCapacity = 10;
    data.crops = (Crop*)malloc(data.cropCapacity * sizeof(Crop));//dynamic memory allocated for crop using malloc

    data.fieldCount = 0;
    data.fieldCapacity = 10;
    data.fields = (Field*)malloc(data.fieldCapacity * sizeof(Field));//dynamic memory allocated for field using malloc

    data.expenseCount = 0;
    data.expenseCapacity = 10;
    data.expenses = (Expense*)malloc(data.expenseCapacity * sizeof(Expense));//dynamic memory allocated for expense using malloc

    loadCropsFromFile(&data);//load crop data from file
    loadFieldsFromFile(&data);//load field data from file
    loadExpensesFromFile(&data);//load expense data from file

    int choice;
    while (1) {//Loop keeps on operating until option 4 is entered that returns 0
        showMainMenu();//Used to display main menu
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();//error handling if user enters anything other than integers

        switch (choice) {
        case 1:
            manageCrops(&data);//redirects user to manage crop section
            break;
        case 2:
            irrigationScheduling(&data);//redirects user to irrigation scheduling section
            break;
        case 3:
            expenseTracking(&data);//redirects user to expense tracking section
            break;
        case 4:
            printf("Exiting program!!!\n");
            saveCropsToFile(&data);//saves crop data before exiting
            saveFieldsToFile(&data);//saves field data before exiting
            saveExpensesToFile(&data);//saves expense data before exiting
            freeData(&data);//frees up used spate using function defined at the end of program
            return 0;
        }
    }
}

// Show main menu
void showMainMenu() {//This is the main menu
    printf(COLOR_BLUE"\n~~~ Smart Farm Management System ~~~ \x1B[0m \n"COLOR_RESET);
    printf("1. Manage Crops\n");
    printf("2. Irrigation Scheduling\n");
    printf("3. Expense Tracking\n");
    printf(COLOR_RED"4. Exit\n"COLOR_RESET);
}

// Manage Crops
void manageCrops(FarmData *data) {//Crop management section
    int choice;
    printf(COLOR_BLUE"\n~~~ Manage Crops ~~~\n"COLOR_RESET);
    printf("1. Add Crop Details\n");
    printf("2. View Crop Details\n");
    printf("3. Display Crop Status\n");
    printf("4. Update Crop Status\n");
    printf(COLOR_RED"5. Delete Crop Details\n"COLOR_RESET);
    printf(COLOR_YELLOW"6. Exit to Main Menu\n"COLOR_RESET);
    printf("Enter your choice : ");
    scanf("%d", &choice);
    clearInputBuffer();

    switch (choice) {
        case 1: 
        addCrop(data);//Call the addCrop function here
        break;
        case 2:
        viewCrops(data); // Call the viewCrops function here
        break;
        case 3:
        displayCropStatuses(data); // Call the displayCropStatuses function here
        break;
        case 4:
        updateCropStatus(data); // Call the updateCropStatus function here
        break;
        case 5:
        deleteCrop(data); // Call the deleteCrop function here
        break;
        case 6:
            printf("Returning to main menu...\n");
        break;
        default:
        clearInputBuffer();
        printf(COLOR_RED"Invalid option. Please try again.\n"COLOR_RESET);
        break;
    }
}
void addCrop(FarmData *data) {//addcrop function to add crop details
    if (data->cropCount == data->cropCapacity) {
        data->cropCapacity *= 2;
        data->crops = (Crop*)realloc(data->crops, data->cropCapacity * sizeof(Crop));
    }

    int addMore = 1;
    while (addMore) {
        Crop newCrop;
        printf("Enter crop name: ");
        scanf("%29s", newCrop.name);
        printf("Enter area planted (in acres): ");
        scanf("%f", &newCrop.area);
        printf("Enter expected yield (in tons): ");
        scanf("%f", &newCrop.yield);

        do {
            printf("Enter planting date (DD/MM/YYYY): ");
            scanf("%10s", newCrop.plantingDate);
            if (!validateDate(newCrop.plantingDate)) {
                printf(COLOR_RED"Invalid date format.\t"COLOR_RESET);
            }
        } while (!validateDate(newCrop.plantingDate));

        do {
            printf("Enter expected harvest date (DD/MM/YYYY): ");
            scanf("%10s", newCrop.harvestDate);
            if (!validateDate(newCrop.harvestDate)) {
                printf(COLOR_RED"Invalid date format.\t"COLOR_RESET);
            }
        } while (!validateDate(newCrop.harvestDate));

        newCrop.status = 1; //Default value  Planted
        data->crops[data->cropCount++] = newCrop;
        printf(COLOR_GREEN"Crop details added successfully!\n"COLOR_RESET);
        printf("Add another crop? (1 = yes, 0 = no): ");
        scanf("%d", &addMore);
    }
}
void viewCrops(FarmData *data) {//View crop function to view crop details
    if (data->cropCount == 0) {
        printf(COLOR_YELLOW"\nNo crops added yet!\n"COLOR_RESET);
        return;
    }

    for (int i = 0; i < data->cropCount; i++) {
        printf("Crop %d:\n", i + 1);
        printf("Name: %s\n", data->crops[i].name);
        printf("Area: %.2f acres\n", data->crops[i].area);
        printf("Yield: %.2f tons\n", data->crops[i].yield);
        printf("Planting Date: %s\n", data->crops[i].plantingDate);
        printf("Harvest Date: %s\n", data->crops[i].harvestDate);
    }
}
// Function to display crop statuses for the main menu of crop management
void displayCropStatuses(FarmData *data) {
    if (data->cropCount == 0) {
        printf(COLOR_YELLOW"No crops added yet!"COLOR_RESET);
        return;
    }
    int i; 
    for (i = 0; i < data->cropCount; i++) {
        printf("Crop %d: ", i + 1);
        switch (data->crops[i].status) {
            case 1:
                printf("Status: Planted\n");
                break;
            case 2:
                printf("Status: Ready to Harvest\n");
                break;
            case 3:
                printf("Status: Harvested\n");
                break;
            default:
                printf(COLOR_RED"Invalid status value!\n"COLOR_RESET);
                break;
        }
    }
}
void updateCropStatus(FarmData *data) {//update crop statuses
    if (data->cropCount == 0) {
        printf(COLOR_YELLOW"No crops added yet!\n"COLOR_RESET);
        return;
    }
    int cropIndex;
    printf("Enter the crop number to update: ");
    scanf("%d", &cropIndex);
    clearInputBuffer();
    cropIndex--; // Adjust for 0-based indexing

    printf("Enter new crop status (1 = Planted, 2 = Ready to Harvest, 3 = Harvested): ");
    scanf("%d", &data->crops[cropIndex].status);
    clearInputBuffer();
    printf(COLOR_GREEN"Crop status updated successfully!\n"COLOR_RESET);
}
void deleteCrop(FarmData *data) {//Delete crop entries
    if (data->cropCount == 0) {
        printf(COLOR_YELLOW"No crops added yet!\n"COLOR_RESET);
        return;
    }
    int cropIndex;
    printf("Enter the crop number to delete: ");
    scanf("%d", &cropIndex);
    clearInputBuffer();
    cropIndex--; 

    for (int i = cropIndex; i < data->cropCount - 1; i++) {
        data->crops[i] = data->crops[i + 1];
    }
    data->cropCount--;
    printf(COLOR_GREEN"Crop details deleted successfully!\n"COLOR_RESET);
}

// Irrigation Scheduling
void irrigationScheduling(FarmData *data){//Irrigation management section
    printf("\n--- Irrigation Scheduling ---\n");
    printf("1. Add field data\n");
    printf("2. View Irrigation Need\n");
    printf("3. Calculate Water Needed\n");
    printf("4. Update Soil Moisture levels\n");
    printf(COLOR_RED"5. Delete Field Details\n"COLOR_RESET);
    printf(COLOR_YELLOW"6. Exit to Main Menu\n"COLOR_RESET);
    printf("Enter your choice : ");
    int choice;
    scanf("%d", &choice);
    clearInputBuffer();

    float area, soilMoisture;
    float waterNeeded;

switch (choice){
    case 1:
        addField(data);//Calls addField function here
        break;
    case 2:
        displayFieldDetails(data);//Calls displayFieldDetails here
        break;
    case 3:
        calculateWaterNeeded(data);//Calls calculateWaterNeeded here
        break;
    case 4:
        updateSoilMoisture(data);//Calls updateSoilMoisture here
        break;
    case 5:
        deleteField(data);//Calls deleteField here
        break;
    case 6:
        printf("Returning to Main Menu...");
        break;
    default:
    clearInputBuffer();
        printf(COLOR_RED"Invalid choice. Please try again.\n"COLOR_RESET);
        break;
    }
}
void addField(FarmData* data) {//add field details
    if(data->fieldCount == data->fieldCapacity){
        data->fieldCapacity *= 2;
        data->fields = (Field*)realloc(data->fields, data->fieldCapacity * sizeof(Field));
    }
    int addMore = 1;
    while(addMore){
        Field newField;
        printf("Enter field type: ");
        scanf("%29s", newField.type);
        printf("Enter area of field (in acres): ");
        scanf("%f", &newField.area);
        printf("Enter initial soil moisture (0-100)%: ");
        scanf("%f", &newField.soilMoisture);

        data->fields[data->fieldCount++] = newField;
        printf(COLOR_GREEN"Field details added successfully!\n"COLOR_RESET);
        
        printf("Add another expense? (1 = yes, 0 = no): ");
        scanf("%d",&addMore);
    }
}

void displayFieldDetails(FarmData* data) {//display field details
    if (data->fieldCount == 0) {
        printf(COLOR_RED"No fields added yet. Please add a field first.\n"COLOR_RESET);
        return;
    }

    for (int i = 0; i < data->fieldCount; i++) {
        Field selectedField = data->fields[i];
        printf("Field %d: Soil moisture: %.2f%%\n", i, selectedField.soilMoisture);

        if (selectedField.soilMoisture < 40) {
            printf(COLOR_RED"Soil moisture is low. Irrigation is recommended.\n"COLOR_RESET);
        } else if (selectedField.soilMoisture < 60) {
            printf(COLOR_YELLOW"Soil moisture is moderate. Monitor soil moisture levels regularly.\n"COLOR_RESET);
        } else {
            printf(COLOR_GREEN"Soil moisture is high. No irrigation is needed.\x1B[0m\n"COLOR_RESET);
        }
        printf("\n");
    }
}

void calculateWaterNeeded(FarmData* data) {//calculate water needed
    if (data->fieldCount == 0) {
        printf(COLOR_RED"No fields added yet. Please add a field first."COLOR_RESET);
        return;
    }
    for (int i = 0; i < data->fieldCount; i++) {
        Field selectedField = data->fields[i];
        float area = selectedField.area;//conversion not needed since formula is independent of unit
        float soilMoisture = selectedField.soilMoisture;
        float waterNeeded = area * (100 - soilMoisture) * 10;//formula to calculate water needed
        printf("Field %d: Water needed: %.0f liters\n", i, waterNeeded);
        printf("\n");
    }
}

void updateSoilMoisture(FarmData* data) {//update soil moisture
    if (data->fieldCount == 0) {
        printf(COLOR_RED"No fields added yet!\n"COLOR_RESET);
        return;
    }
    int fieldIndex;
    printf("Enter the field number to update: ");
    scanf("%d", &fieldIndex);
    clearInputBuffer();
    fieldIndex--; // Adjust for 0-based indexing

    printf("Enter new soil moisture (0-100)%: ");
    scanf("%f", &data->fields[fieldIndex].soilMoisture);
    clearInputBuffer();
    printf(COLOR_GREEN"Soil moisture updated successfully!\n"COLOR_RESET);
}

void deleteField(FarmData* data) {//delete field entry
    if (data->fieldCount == 0) {
        printf(COLOR_RED"No fields added yet!\n"COLOR_RESET);
        return;
    }
    int fieldIndex;
    printf("Enter the field number to delete: ");
    scanf("%d", &fieldIndex);
    clearInputBuffer();
    fieldIndex--; // Adjust for 0-based indexing

    for (int i = fieldIndex; i < data->fieldCount - 1; i++) {
        data->fields[i] = data->fields[i + 1];
    }
    data->fieldCount--;
    printf(COLOR_GREEN"Field details deleted successfully!\n"COLOR_RESET);
}
// Expense Tracking
    void expenseTracking(FarmData *data) {//expense management section
    printf(COLOR_BLUE"\n~~~ Expense Tracking ~~~\n"COLOR_RESET);
    printf("1. Add New Expense\n");
    printf("2. View All Expenses\n");
    printf("3. Calculate Total and Average Expenses\n");
    printf(COLOR_RED"4. Delete Expense Details\n"COLOR_RESET);
    printf(COLOR_YELLOW"5. Exit to Main Menu\n"COLOR_RESET);
    printf("Enter your choice : ");
    int choice;
    scanf("%d", &choice);
    clearInputBuffer();

    switch (choice) {
    case 1:
        addExpense(data);//Calls addExpense here
        break;
    case 2:
        viewExpenses(data);//Calls viewExpenses here
        break;
    case 3:
        calculateTotalAndAverageExpenses(data);//Calls calculateTotalAndAverageExpenses here
        break;
    case 4:
        deleteExpense(data);//Calls deleteExpense here
        break;
    case 5:
        printf("Returning to Main Menu...");
        break;
    default:
        clearInputBuffer();
        printf(COLOR_RED"Invalid choice. Please try again.\n"COLOR_RESET);
        break;
     }
}
void addExpense(FarmData *data) {//add expense details
    if (data->expenseCount == data->expenseCapacity) {
        data->expenseCapacity *= 2;
        data->expenses = (Expense*) realloc(data->expenses, data->expenseCapacity * sizeof(Expense));
    }
    int addMore = 1;
    while(addMore){
        Expense newExpense;
        printf("Enter expense name: ");
        scanf("%19s", newExpense.name);
        printf("Enter expense category: ");
        scanf("%29s", newExpense.category);
        printf("Enter expense amount: ");
        scanf("%f", &newExpense.amount);

        data->expenses[data->expenseCount++] = newExpense;
        printf(COLOR_GREEN"Expense details added successfully!\n"COLOR_RESET);
        
        printf("Add another expense? (1 = yes, 0 = no): ");
        scanf("%d",&addMore);
    }
}

void viewExpenses(FarmData *data) {//view expense details here
    if (data->expenseCount == 0) {
        printf(COLOR_RED"No expenses added yet!\n"COLOR_RESET);
        return;
    }

    printf("All Expenses:\n");
    for (int i = 0; i < data->expenseCount; i++) {
        printf("Expense %d:\n", i + 1);
        printf("Name: %s\n", data->expenses[i].name);
        printf("Category: %s\n", data->expenses[i].category);
        printf("Amount: %.2f\n", data->expenses[i].amount);
        printf("\n");
    }
}

void calculateTotalAndAverageExpenses(FarmData *data) {//calculate overall total and average
    if (data->expenseCount == 0) {
        printf(COLOR_RED"No expenses added yet!\n"COLOR_RESET);
        return;
    }

    float totalExpenses = 0;

    for (int i = 0; i < data->expenseCount; i++) {
        totalExpenses += data->expenses[i].amount;
    }

    printf("Total expenses: %.2f\n", totalExpenses);
    printf("Average expenses: %.2f\n", totalExpenses / data->expenseCount);
}

void deleteExpense(FarmData *data) {//delete expense details
    if (data->expenseCount == 0) {
        printf(COLOR_RED"No expenses added yet!\n"COLOR_RESET);
        return;
    }

    int expenseIndex;
    printf("Enter the expense number to delete: ");
    scanf("%d", &expenseIndex);
    clearInputBuffer();
    expenseIndex--; 

    for (int i = expenseIndex; i < data->expenseCount - 1; i++) {
        data->expenses[i] = data->expenses[i + 1];
    }
    data->expenseCount--;
    printf(COLOR_GREEN"Expense details deleted successfully!\n"COLOR_RESET);
}
// Save Crops to File
void saveCropsToFile(FarmData *data) {
    FILE *file = fopen("crops.txt", "w");
    if (file == NULL) {
        printf(COLOR_RED"Error opening file!\n"COLOR_RESET);
        return;
    }
    for (int i = 0; i < data->cropCount; i++) {
        fprintf(file, "%s %.2f %.2f %s %s %d\n",
                data->crops[i].name, data->crops[i].area, data->crops[i].yield,
                data->crops[i].plantingDate, data->crops[i].harvestDate, data->crops[i].status);
    }
    fclose(file);
}
// Save Fields to File
void saveFieldsToFile(FarmData *data) {
    FILE *file = fopen("fields.txt", "w");
    if (file == NULL) {
        printf(COLOR_RED"Error opening file!\n"COLOR_RESET);
        return;
    }
    for (int i = 0; i < data->fieldCount; i++) {
        fprintf(file, "%s %.2f %.2f%\n",
                data->fields[i].type, data->fields[i].area, data->fields[i].soilMoisture);
    }
    fclose(file);
}
// Save expenses to file
void saveExpensesToFile(FarmData *data) {
    FILE *file = fopen("expenses.txt", "w");
    if (file == NULL) {
        printf(COLOR_RED"Error opening file!\n"COLOR_RESET);
        return;
    }
    for (int i = 0; i < data->expenseCount; i++) {
        fprintf(file, "%s,%s,$%.2f\n", data->expenses[i].name, data->expenses[i].category, data->expenses[i].amount);
    }
    fclose(file);
 }
// Load Crops from File
void loadCropsFromFile(FarmData *data) {
    FILE *file = fopen("crops.txt", "r");
    if (file == NULL) {
        printf(COLOR_RED"Error opening file!\n"COLOR_RESET);
        return;
    }
    data->cropCount = 0;
    while (1) {
        Crop newCrop;
        if (fscanf(file, "%29s %f %f %10s %10s %d",
                newCrop.name, &newCrop.area, &newCrop.yield,
                newCrop.plantingDate, newCrop.harvestDate, &newCrop.status) != 6) {
            break;
        }
        if (data->cropCount == data->cropCapacity) {
            data->cropCapacity *= 2;
            data->crops = (Crop*)realloc(data->crops, data->cropCapacity * sizeof(Crop));
        }
        data->crops[data->cropCount++] = newCrop;
    }
    fclose(file);
}
// Load Fields from File
void loadFieldsFromFile(FarmData *data) {
    FILE *file = fopen("fields.txt", "r");
    if (file == NULL) {
        printf(COLOR_RED"Error opening file!\n"COLOR_RESET);
        return;
    }
    data->fieldCount = 0;
    while (1) {
        Field newField;
        if (fscanf(file, "%29s %f %f",
                newField.type, &newField.area, &newField.soilMoisture) != 3) {
            break;
        }
        if (data->fieldCount == data->fieldCapacity) {
            data->fieldCapacity *= 2;
            data->fields = (Field*)realloc(data->fields, data->fieldCapacity * sizeof(Field));
        }
        data->fields[data->fieldCount++] = newField;
    }
    fclose(file);
}
// Load Expenses from File
void loadExpensesFromFile(FarmData *data) {
    FILE *file = fopen("expenses.txt", "r");
    if (file == NULL) {
        printf(COLOR_RED"Error opening file!\n"COLOR_RESET);
        return;
    }
    data->expenseCount = 0;
    while (1) {
        Expense newExpense;
        char date[11];
        if (fscanf(file, "%19s %29s %f ",
                newExpense.name, newExpense.category, &newExpense.amount) != 3) {
            break;
        }
        if (data->expenseCount == data->expenseCapacity) {
            data->expenseCapacity *= 2;
            data->expenses = (Expense*)realloc(data->expenses, data->expenseCapacity * sizeof(Expense));
        }
        data->expenses[data->expenseCount++] = newExpense;
    }
    fclose(file);
}

// Clear Input Buffer->when user inputs anything it is temporarily stored/
//When it is stored in the Buffer, error is handled using this function/
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Free Data for dynamic memory management
void freeData(FarmData *data) {
    free(data->crops);
    free(data->fields);
    free(data->expenses);
}

// Validate Date in the format DD/MM/YYYY
int validateDate(const char *date) {
    int day, month, year;
    if (sscanf(date, "%2d/%2d/%4d", &day, &month, &year) != 3) {
        return 0;
    }

    if (day < 1 || day > 31 || month < 1 || month > 12) {
        return 0;
    }

    return 1;
}
