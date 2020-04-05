/*
 * Copy the contents of header.txt (filled out) and paste here
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Set.h"

/*
 * Several of the required functions have been written already
 * You may use these functions as-is or you may modify them in any way.
 * Please NOTE, you are responsible for ensuring that the functions work correctly in
 * your project solution. Just because the function works correctly in my solution does
 * NOT mean that the function will work correctly in yours.
 */

/*
 * Design NOTES:
 *
 * The design provided in this starter kit assumes
 * (1) empty sets will be represented with length == 0 and elements == nullptr (i.e., address 0)
 * (2) amortized doubling is not used, and capacity is ignored/unused. Functions should assume that
 * the amount of storage available in the elements[] array is equal to length
 */



/* done for you already */
void destroySet(Set* self) {
    free(self->elements);
}

/* done for you already */
void createEmptySet(Set* self) {
    self->len = 0;
    self->elements = nullptr;
}

/* done for you already */
void createSingletonSet(Set* self, int x) {
    self->elements = (int*) malloc(sizeof(int));
    self->elements[0] = x;
    self->len = 1;
}

/* done for you already */
void createCopySet(Set* self, const Set* other) {
    self->elements = (int*) malloc(other->len * sizeof(int));
    for (int k = 0; k < other->len; k += 1) {
        self->elements[k] = other->elements[k];
    }
    self->len = other->len;
}

/* done for you already */
void assignSet(Set* self, const Set* other) {
    if (self == other) { return; }

    destroySet(self);
    createCopySet(self, other);
}

bool isMemberSetHelper(int* elements, int min, int max, int num) {
    int index = (max + min)/2;
    if(elements[index] == num) {
        return true;
    }
    else if(num > elements[index]) {
        min = index;
    }
    else if(num < elements[index]) {
        max = index;
    }
    if(max == min) {
        return false;
    }
    isMemberSetHelper(elements, min, max, num);
}

/* return true if x is an element of self O(logn) */
bool isMemberSet(const Set* self, int x) {
    return isMemberSetHelper(self->elements, 0, self->len - 1, x);
}

/*
 * add x as a new member to this set.
 * If x is already a member, then self should not be changed
 * Be sure to restore the design invariant property that elements[] remains sorted
 * (yes, you can assume it is sorted when the function is called, that's what an invariant is all about)
 */

int findIndex(Set* self, int x) {
    int index;
    if(x < self->elements[index]) {
        index = 0;
    }
    else if(x > self->elements[self->len - 1]) {
        index = self->len;
    }
    else {
        for(index = 1; index < self->len; index++) {
            if(x > self->elements[index-1] && x < self->elements[index + 1]) break;
        }
    }
    return index;
}

void insertSet(Set* self, int x) {
    if(isMemberSet(self, x)) return;
    int index = findIndex(self, x);
    self->len += 1;
    self->elements = (int*) realloc(self->elements, self->len * 4);
    for(int i = self->len - 1; i > index; i++) {
        self->elements[i] = self->elements[i-1];
    }
    self->elements[index] = x;
}


/*
 * don't forget: it is OK to try to remove an element
 * that is NOT in the set.
 * If 'x' is not in the set 'self', then
 * removeSet should do nothing (it's not an error)
 * Otherwise, ('x' IS in the set), remove x. Be sure to update self->length
 * It is not necessary (nor recommended) to call malloc -- if removing an element means the
 * array on the heap is "too big", that's almost certainly OK, and reallocating a smaller array
 * is almost definitely NOT worth the trouble
 */
void removeSet(Set* self, int x) {
    if(!isMemberSet(self, x)) return;
    int index = findIndex(self, x);
    self->len -= 1;
    for(int i = index; i < self->len; i++) {
        self->elements[i] = self->elements[i+1];
    }
    self->elements[self->len] = 0;
    self->elements = (int*) realloc(self->elements, self->len * 4);
}

/* done for you already */
void displaySet(const Set* self) {
    int k;

    printf("{");

    if (self->len == 0) {
        printf("}");
    }
    else {
        for (k = 0; k < self->len; k += 1) {
            if (k < self->len - 1) {
                printf("%d,", self->elements[k]);
            } else {
                printf("%d}", self->elements[k]);
            }
        }
    }
}


/* return true if self and other have exactly the same elements */
bool isEqualToSet(const Set* self, const Set* other) {
    if(self->len != other->len) return false;
    for(int i = 0; i < self->len; i++) {
        if(self->elements[i] != other->elements[i]) return false;
    }
    return true;
}

/* return true if every element of self is also an element of other */
bool isSubsetOf(const Set* self, const Set* other) {
    int index = 0;
    for(; index < other->len; index++) {
        if(self->elements[0] == other->elements[index]) break;
    }
    if(index + self->len + 1> other->len) return false;
    for(int i = 0; i < self->len; i++) {
        if(self->elements[i] != other->elements[index + i]) return false;
    }
    return true;
}

/* done for you */
bool isEmptySet(const Set* self) {
    return self->len == 0;
}

/* remove all elements from self that are not also elements of other */
void intersectFromSet(Set* self, const Set* other) {
    int* out = (int*) malloc(sizeof(int) * self->len);
    int selfIndex = 0;
    int otherIndex = 0;
    int outIndex = 0;
    for(int i = 0; i < self->len + other->len; i++) {
        if(self->elements[selfIndex] == other->elements[otherIndex]) {
            out[outIndex] = self->elements[selfIndex];
            outIndex++;
            if(selfIndex < self->len - 1) selfIndex++;
            if(otherIndex < other->len - 1) otherIndex++;
        }
        else if(self->elements[selfIndex] > other->elements[otherIndex]) {
            if(otherIndex < other->len - 1) otherIndex++;
        }
        else if(self->elements[selfIndex] < other->elements[otherIndex]) {
            if(selfIndex < self->len - 1) selfIndex++;
        }
    }
    out = (int*) realloc(out, sizeof(int) * outIndex);
    self->len = outIndex;
    free(self->elements);
    self->elements = out;
}

/* remove all elements from self that are also elements of other */
void subtractFromSet(Set* self, const Set* other) {
    int* out = (int*) malloc(sizeof(int) * self->len);
    int selfIndex = 0;
    int otherIndex = 0;
    int outIndex = 0;
    for(int i = 0; i < self->len + other->len; i++) {
        if(self->elements[selfIndex] != other->elements[otherIndex]) {
            out[outIndex] = self->elements[selfIndex];
            outIndex++;
            if(selfIndex < self->len - 1) selfIndex++;
            if(otherIndex < other->len - 1) otherIndex++;
        }
        else if(self->elements[selfIndex] > other->elements[otherIndex]) {
            if(otherIndex < other->len - 1) otherIndex++;
        }
        else if(self->elements[selfIndex] < other->elements[otherIndex]) {
            if(selfIndex < self->len - 1) selfIndex++;
        }
    }
    out = (int*) realloc(out, sizeof(int) * outIndex);
    self->len = outIndex;
    free(self->elements);
    self->elements = out;
}

/* add all elements of other to self (obviously, without creating duplicate elements) */
void unionInSet(Set* self, const Set* other) {
    int* out = (int*) malloc(sizeof(int) * (self->len + other->len));
    int selfIndex = 0;
    int otherIndex = 0;
    int outIndex = 0;
    for(int i = 0; i < self->len + other->len; i++) {

    }
}
