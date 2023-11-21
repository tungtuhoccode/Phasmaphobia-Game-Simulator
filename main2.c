#include "defs.h"

int main()
{
    // Initialize the random number generator
    srand(time(NULL));

    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures
    // HouseType house;

    // populateRooms(&house);

    //Initilize
    HouseType *house;
    initHouse(&house);
    populateRooms(house); 
    printRoomList(&house->rooms);

    HunterType* hunters[NUM_HUNTERS]; 
    createNewHunters(hunters, &house->sharedEvList);

    GhostType *ghost;
    initGhost(&ghost);
    placeGhostInRandomRoom(ghost, house);
    l_ghostInit(ghost->ghostClass, ghost->inRoom->roomName);

    placeHuntersInFirstRoom(house, hunters);
    printHuntersInHouse(house);

    addHunterToRoom(house->rooms.head->next->data, house->huntersInHouse[0]);
    addHunterToRoom(house->rooms.head->next->data, house->huntersInHouse[1]);
    addHunterToRoom(house->rooms.head->next->data, house->huntersInHouse[2]);
    addHunterToRoom(house->rooms.head->next->data, house->huntersInHouse[3]);

    printHuntersInRoom(house->rooms.head->next->data);


    //Thread creation
    //Simnulation started
    pthread_t ghostThread, hunterThread[NUM_HUNTERS];
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n*******SIMULATION STARTED*******\n\n\n");

    pthread_create(&ghostThread, NULL, runGhostSimulationThread, ghost);
    for (int i =0;i<NUM_HUNTERS;i++){
        pthread_create(&hunterThread[i], NULL, runHunterSimulationThread, hunters[i]);
    }


    pthread_join(ghostThread, NULL);
    // pthread_join(hunterThread[0], NULL);
    for (int i =0;i<NUM_HUNTERS;i++){
        pthread_join(hunterThread[i], NULL);
    }

    // testEvidenceRemoveAndAdd();


    // RoomNodeType *currNode = house->rooms.head;
    // while(currNode != NULL){
    //     printf("Connected room to %s are:\n", currNode->data->roomName);
    //     printRoomList(&currNode->data->connectedRooms);
    //     printf("\n\n-----------------------\n\n");
    //     currNode = currNode->next;
    // }


    //free
    freeGhost(ghost);
    freeHunterList(hunters);
    freeHouse(house);

    return 0;
}

void testEvidenceRemoveAndAdd(){
    printf("TEST START\n\n\n");
    EvidenceStructType *ev;
    EvidenceListType evList;

    initEvidenceList(&evList);

    initEvidence(&ev, EMF);
    addEvidenceToList(&evList, ev);
    printEvidenceList(&evList);

    initEvidence(&ev, SOUND);
    addEvidenceToList(&evList, ev);
    printEvidenceList(&evList);

    initEvidence(&ev, FINGERPRINTS);
    addEvidenceToList(&evList, ev);
    printEvidenceList(&evList);

    initEvidence(&ev, TEMPERATURE);
    addEvidenceToList(&evList, ev);
    printEvidenceList(&evList);

    printf("\n\ntest remove SOUND\n");
    initEvidence(&ev, SOUND);
    removeEvidenceFromList(&evList, ev);

    // printf("\n\ntest remove EMF\n");
    // initEvidence(&ev, EMF);
        // removeEvidenceFromList(&evList, ev);

    // printf("\n\ntest remove TEMPERATURE\n");
    // initEvidence(&ev, TEMPERATURE);
        // removeEvidenceFromList(&evList, ev);

    printf("\n\ntest remove FINGERPRINTS\n");
    initEvidence(&ev, FINGERPRINTS);
    removeEvidenceFromList(&evList, ev);
    printEvidenceList(&evList);
    printf("END TEST\n\n\n");

}

void* runGhostSimulationThread(void* arg){   
    GhostType *ghost = (GhostType*) arg;
    // printf("Ghost thread is running\n");
    // printf("Ghost thread is running2");
    if(checkHunterInRoom(ghost->inRoom) == C_TRUE){
        ghost->boredomTimer = 0;
    }

}

void* runHunterSimulationThread(void* arg){   
    HunterType* hunter = (HunterType*) arg;
    testMove(hunter);

    return 0;
}

void testMove(HunterType *hunter){
    RoomType *hunterOldRoom = hunter->currentRoom;
    
    sleep(randInt(1,5));
    moveHunter(hunter, &hunter->currentRoom->connectedRooms, C_TRUE);
    printf("all hunter in old room (%s): \n", hunterOldRoom->roomName);
    printHuntersInRoom(hunterOldRoom);
    printf("\nall hunter in new room: (%s) \n", hunter->currentRoom->roomName);
    printHuntersInRoom(hunter->currentRoom);
    printf("-----------------------------------\n");

    sleep(randInt(1,5));
    hunterOldRoom = hunter->currentRoom;
    moveHunter(hunter, &hunter->currentRoom->connectedRooms, C_FALSE);
    printf("all hunter in old room (%s): \n", hunterOldRoom->roomName);
    printHuntersInRoom(hunterOldRoom);
    printf("\nall hunter in new room: (%s) \n", hunter->currentRoom->roomName);
    printHuntersInRoom(hunter->currentRoom);
    printf("-----------------------------------\n");

    sleep(randInt(1,5));
    hunterOldRoom = hunter->currentRoom;
    moveHunter(hunter, &hunter->currentRoom->connectedRooms, C_FALSE);
    printf("all hunter in old room (%s): \n", hunterOldRoom->roomName);
    printHuntersInRoom(hunterOldRoom);
    printf("\nall hunter in new room: (%s) \n", hunter->currentRoom->roomName);
    printHuntersInRoom(hunter->currentRoom);

    printf("-----------------------------------\n");

}

//get random number in range (0,max-1)
int getRandomInRange(int max){
    return rand() % max;
}



