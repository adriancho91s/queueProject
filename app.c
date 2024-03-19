#include <stdio.h>
#include <stdlib.h>

// Macro to clear the console screen
#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif


/**
 * @brief Represents a person with various attributes.
 */
typedef struct {
    int identification;         /**< The identification number of the person. */
    char lastName[50];          /**< The last name of the person. */
    char firstName[50];         /**< The first name of the person. */
    int age;                    /**< The age of the person. */
    char gender;                /**< The gender of the person. */
    char phoneNumber[15];       /**< The phone number of the person. */
    char serviceDate[20];       /**< The service date of the person. */
} Person;

/**
 * @brief Represents a node in a linked list.
 */
typedef struct Node {
    Person person;              /**< The person stored in the node. */
    struct Node *next;          /**< Pointer to the next node in the list. */
} Node;


/**
 * Enqueues a person into the queue.
 *
 * @param queue A pointer to the pointer of the queue.
 * @param person The person to be enqueued.
 */
void enqueue(Node **queue, Person person) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->person = person;
    new_node->next = NULL;

    if (*queue == NULL) {
        *queue = new_node;
    } else {
        Node *current = *queue;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

/**
 * Pushes a new element onto the stack.
 *
 * @param stack A pointer to the top of the stack.
 * @param person The person to be added to the stack.
 */
void push(Node **stack, Person person) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->person = person;
    new_node->next = *stack;
    *stack = new_node;
}

/**
 * Adds a person to the appropriate priority queue based on their age.
 *
 * @param priority1 A pointer to the head of the priority 1 queue.
 * @param priority2 A pointer to the head of the priority 2 queue.
 * @param priority3 A pointer to the head of the priority 3 queue.
 * @param person The person to be added to the queue.
 */
void addPerson(Node **priority1, Node **priority2, Node **priority3, Person person) {
    if (person.age > 55) {
        enqueue(priority1, person);
    } else if (person.age > 40) {
        enqueue(priority2, person);
    } else {
        enqueue(priority3, person);
    }
}

/**
 * Dequeues a person from the queue.
 *
 * @param queue A pointer to the pointer of the queue.
 * @return The person that was dequeued.
 */
Person dequeue(Node **queue) {
    if (*queue == NULL) {
        printf("The queue is empty\n");
        exit(1);
    }
    Node *temp = *queue;
    Person person = temp->person;
    *queue = temp->next;
    free(temp);
    return person;
}


/**
 * @brief Function to attend a person from the queue.
 *
 * @param queue A pointer to the queue.
 * @param stack A pointer to the stack.
 */
void attendPerson(Node **queue, Node **stack) {
    Person person = dequeue(queue);
    //if the person is not in the queue, the function will return
    //Show the person's information to be attended
    printf("The person to be attended is: %s %s\n", person.firstName, person.lastName);
    printf("Age: %d\n", person.age);
    printf("Phone number: %s\n", person.phoneNumber);
    printf("Service date: %s\n", person.serviceDate);
    //Add the person to the stack
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->person = person;
    new_node->next = *stack;
    *stack = new_node;
}


/**
 * Moves to the next person in the queue based on their priority.
 *
 * @param peopleAttend Pointer to the counter for the number of people attended.
 * @param peoplePriority Pointer to the current priority level.
 * @param priority1 Pointer to the first priority queue.
 * @param priority2 Pointer to the second priority queue.
 * @param priority3 Pointer to the third priority queue.
 * @param stack Pointer to the stack used for attending people.
 */
void nextPerson(int *peopleAttend, int *peoplePriority, Node **priority1, Node **priority2, Node **priority3, Node **stack) {
    // Check if all queues are empty
    if (*priority1 == NULL && *priority2 == NULL && *priority3 == NULL) {
        printf("The queues are empty\n");
        return;
    }

    // Check if the current priority queue has reached its limit or is empty
    if ((*peoplePriority == 1 && (*peopleAttend == 3 || *priority1 == NULL)) ||
        (*peoplePriority == 2 && (*peopleAttend == 1 || *priority2 == NULL)) ||
        (*peoplePriority == 3 && (*peopleAttend == 1 || *priority3 == NULL))) {
        // If so, move to the next priority queue and reset the attend counter
        *peoplePriority = (*peoplePriority % 3) + 1; // To prevent the peoplePriority from being greater than 3
        *peopleAttend = 0;
    }

    // Attend to the person based on the current priority, if the queue is not empty
    if (*peoplePriority == 1 && *priority1 != NULL) {
        attendPerson(priority1, stack);
        *peopleAttend += 1;
    } else if (*peoplePriority == 2 && *priority2 != NULL) {
        attendPerson(priority2, stack);
        *peopleAttend += 1;
    } else if (*peoplePriority == 3 && *priority3 != NULL) {
        attendPerson(priority3, stack);
        *peopleAttend += 1;
    } else {
        // If the current priority queue is empty, move to the next priority queue and reset the attend counter
        *peoplePriority = (*peoplePriority % 3) + 1;
        *peopleAttend = 0;
        // Try the next queue before ending the execution recursively
        nextPerson(peopleAttend, peoplePriority, priority1, priority2, priority3, stack);
    }
}

/**
 * Finds the position of a node with a specific identification in a queue.
 *
 * @param queue The queue to search in.
 * @param identification The identification to search for.
 * @return The position of the node with the given identification in the queue, or -1 if not found.
 */
int findPositionInQueue(Node *queue, int identification) {
    int position = 1;
    Node *current = queue;
    while (current != NULL) {
        if (current->person.identification == identification) {
            return position;
        }
        current = current->next;
        position++;
    }
    return -1;
}

/**
 * Removes a person from the queue at the specified position.
 *
 * @param position The position of the person to be removed.
 * @param queue A pointer to the head of the queue.
 */
void removePerson(int position, Node **queue) {
    if (position == 1) {
        Node *temp = *queue;
        *queue = temp->next;
        free(temp);
    } else {
        Node *current = *queue;
        for (int i = 1; i < position - 1; i++) {
            current = current->next;
        }
        Node *temp = current->next;
        current->next = temp->next;
        free(temp);
    }
}

/**
 * Checks if a person with the given identification and age is in the queue.
 * 
 * @param identification The identification number of the person.
 * @param age The age of the person.
 * @param priority1 The head of the queue for priority 1.
 * @param priority2 The head of the queue for priority 2.
 * @param priority3 The head of the queue for priority 3.
 * @return 1 if the person is in the queue, 0 otherwise.
 */
int personInQueue(int identification, int age, Node *priority1, Node *priority2, Node *priority3) {
    if (age > 55) {
        return findPositionInQueue(priority1, identification) != -1;
    } else if (age > 40) {
        return findPositionInQueue(priority2, identification) != -1;
    } else {
        return findPositionInQueue(priority3, identification) != -1;
    }
}

/**
 * Removes a person from the queue based on their identification and age.
 * If the person's age is greater than 55, they are removed from priority1 queue.
 * If the person's age is greater than 40, they are removed from priority2 queue.
 * Otherwise, they are removed from priority3 queue.
 *
 * @param identification The identification of the person to be removed.
 * @param age The age of the person to be removed.
 * @param priority1 Pointer to the head of the priority1 queue.
 * @param priority2 Pointer to the head of the priority2 queue.
 * @param priority3 Pointer to the head of the priority3 queue.
 */
void removePersonFromQueue(int identification, int age, Node **priority1, Node **priority2, Node **priority3) {
    if (age > 55) {
        int position = findPositionInQueue(*priority1, identification);
        if (position != -1) {
            removePerson(position, priority1);
        }
    } else if (age > 40) {
        int position = findPositionInQueue(*priority2, identification);
        if (position != -1) {
            removePerson(position, priority2);
        }
    } else {
        int position = findPositionInQueue(*priority3, identification);
        if (position != -1) {
            removePerson(position, priority3);
        }
    }
}

/**
 * Displays the people in attendance by traversing the given stack.
 *
 * @param stack The stack containing the people in attendance.
 */
void showPeopleInAttendance(Node *stack) {
    Node *current = stack;
    if (current == NULL) {
        printf("There are no people in attendance\n");
        return;
    }
    while (current != NULL) {
        printf("-------------------------------------------------\n");
        printf("| The person in attendance is: %s %s\n", current->person.firstName, current->person.lastName);
        printf("| Age: %d\n", current->person.age);
        printf("| Phone number: %s\n", current->person.phoneNumber);
        printf("| Service date: %s\n", current->person.serviceDate);
        printf("-------------------------------------------------\n");
        current = current->next;
    }
}

/**
 * Saves the people in attendance to a file.
 * 
 * @param stack The stack containing the people in attendance.
 */
void savePeopleInAttendance(Node *stack) {
    FILE *file = fopen("peopleAttended.dat", "a+");
    if (file == NULL) {
        printf("The file could not be opened\n");
        exit(1);
    }
    Node *current = stack;
    while (current != NULL) {
        fwrite(&current->person, sizeof(Person), 1, file);
        current = current->next;
    }
    fclose(file);
}


/**
 * Reads the people in attendance from a file and pushes them onto a stack.
 * 
 * @param stack A pointer to the stack where the people will be pushed.
 */
void readPeopleInAttendance(Node **stack) {
    FILE *file = fopen("peopleAttended.dat", "r");
    if (file == NULL) {
        return;
    }
    Person person;
    while (fread(&person, sizeof(Person), 1, file)) {
        push(stack, person);
    }
    fclose(file);
}

/**
 * Frees the memory allocated for the priority queues and stack.
 * 
 * @param priority1 A pointer to the head of the first priority queue.
 * @param priority2 A pointer to the head of the second priority queue.
 * @param priority3 A pointer to the head of the third priority queue.
 * @param stack A pointer to the head of the stack.
 */
void freeMemory(Node **priority1, Node **priority2, Node **priority3, Node **stack) {
    Node *current = *priority1;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    current = *priority2;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    current = *priority3;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    current = *stack;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
}

/**
 * Displays the menu options for the hospital management system.
 */
void showMenu() {
    printf("Hospital management system\n");
    printf("1. Add new person to be attend\n");
    printf("2. Attend next person\n");
    printf("3. Remove person from the queue\n");
    printf("4. Show people in attendance\n");
    printf("5. Exit\n");
    printf("Enter an option: ");
}

/**
 * Clears the screen by executing the system command to clear the console.
 */
void clearScreen() {
    system(CLEAR);
}


int main() {
    Node *priority1 = NULL;
    Node *priority2 = NULL;
    Node *priority3 = NULL;
    Node *stack = NULL;
    int peopleAttend = 0;
    int peoplePriority = 1;
    int option;
    readPeopleInAttendance(&stack);

    do {
        showMenu();
        scanf("%d", &option);
        clearScreen();
        switch (option) {
            case 1: {
                Person person;
                printf("Enter the person's identification: ");
                scanf("%d", &person.identification);
                printf("Enter the person's first name: ");
                scanf("%s", person.firstName);
                printf("Enter the person's last name: ");
                scanf("%s", person.lastName);
                printf("Enter the person's age: ");
                scanf("%d", &person.age);
                printf("Enter the person's gender: ");
                scanf(" %c", &person.gender);
                printf("Enter the person's Phone number: ");
                scanf("%s", person.phoneNumber);
                printf("Enter the person's service date: ");
                scanf("%s", person.serviceDate);
                addPerson(&priority1, &priority2, &priority3, person);
                clearScreen();

                printf("The person has been added to the queue\n");
                printf("Press enter to continue\n");
                getc(stdin);
                getc(stdin);
                clearScreen();
                break;
            }
            case 2: {
                nextPerson(&peopleAttend, &peoplePriority, &priority1, &priority2, &priority3, &stack);
                printf("Press enter to continue...\n");
                getc(stdin);
                getc(stdin);
                clearScreen();
                break;
            }
            case 3: {
                int identification;
                int age;
                printf("Enter the person's identification: ");
                scanf("%d", &identification);
                printf("Enter the person's age: ");
                scanf("%d", &age);
                if (personInQueue(identification, age, priority1, priority2, priority3)) {
                    removePersonFromQueue(identification, age, &priority1, &priority2, &priority3);
                    clearScreen();
                    printf("The person with ID:%d has been removed from the queue\n", identification);
                } else {
                    printf("The person is not in the queue\n");
                }
                printf("Press enter to continue\n");
                getc(stdin);
                getc(stdin);
                clearScreen();
                break;
            }
            case 4: {
                showPeopleInAttendance(stack);
                printf("Press enter to continue\n");
                getc(stdin);
                getc(stdin);
                clearScreen();
                break;
            }
            case 5: {
                savePeopleInAttendance(stack);
                freeMemory(&priority1, &priority2, &priority3, &stack);
                break;
            }
            default: {
                printf("Invalid option\n");
                break;
            }
        }
    } while (option != 5);

    return 0;
}