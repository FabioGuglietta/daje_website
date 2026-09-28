#include <stdio.h>
#include <malloc.h>

int main(void){
        char     a[10] = "Test1"; // this is writable, at most 10 char (80 bytes)

        char    *b     = "Test2"; // this is read-only, will be allocated at 
                                  // run time reserving the required space
                                  // in the text segment. The pointer can be
                                  // resused in the same way as char *d below.

        char     c[]   = "Test3"; // this is writable, will be allocated at 
                                  // run time reserving the required space

        char    *d              ; // this points to an arbitrarily-long sequence
                                  // of chars and its target needs to be alllocated,
                                  // either statically or dynamically.

        char (*e)[10]           ; // this is a pointer to an array of 10 chars, which itself 
                                  // will be a pointer to a memory region containing the string.

        char    *f[10]          ; // these are 10 pointers to char, also more clearly written
                                  // as (char*)e[10]
                                  
        char ** g               ; // this is a pointer to a pointer, similar to what we have with e, 
                                  // with the difference that what's pointed by **g (*g) is a pointer itself
                                  // and can be allocated.

        printf("char a[10] = \"Test1\"\n");
        printf("a points to the string: '%s' starting at %p (note the range of address 0x7, on the stack!)\n",a,a);
        printf("a[4] = 'X'\n");
        a[4]='X';
        printf("a points to the string: '%s' starting at %p (note the same address) \n\n",a,a);

        printf("char *b = \"Test2\"\n");
        printf("b points the string: '%s' starting at %p \n",b,b);
        printf("b = \"TestY\"\n");
        b = "TestY";
        printf("b points to the string: '%s' starting at %p (note the change of address and range, 0x5, on the heap!)\n\n",b,b);
        // b[4]='X'; // this would segfaults because b is stored in the read-only memory section
        printf("b[4]='X' would segfault, as b points to memory in the read-only text segment!\n");
        printf("Note that b itself is at the address &b -> %p on the stack! It's the string %s pointed by b that's on the heap...\n\n",&b,b);

        printf("char c[]   = \"Test3\"\n");
        printf("c points to the string: '%s' starting at %p (note the address range, 0x7, on the stack! which is writable  - compare with b)\n\n",c,c);
        printf("c[4] = 'Z'\n");
        c[4]='Z';
        printf("c points to the string: '%s' starting at %p (note the same address, it's writable )\n\n",c,c);
    

        printf("d = &a[0]\n");
        d = &a[0]; //  c now points to a memory region that has been allocated statically (a).
                   //  Equivalently, we could have written c = a ;            
        printf("d points to the string: '%s'      starting at %p, which is the same as that of a (%p) \n",d,d,&a);

        printf("d = a\n");
        d = a;     //  d now points to a memory region that has been allocated statically (a).
        printf("d points to the string: '%s'      starting at %p, which is the same as that of a (%p) \n\n",d,d,&a); // note &a

        printf("d = b\n");
        d = b; //  d now points to a memory region that has been allocated statically (b). Note 
               //  that we do not need to pass the address of the first element of b (although we could).
        printf("d points to the string: '%s'      starting at %p, which is the same as that of b (%p) \n\n",d,d,b);  // note b, not &b
        printf("d = (char*) malloc(10* sizeof(char))\n");
        d = (char*) malloc(10* sizeof(char)); // d now points to a memory region that has been allocated dynamically.
        printf("d[0]='\\0'\n");
        d[0]='\0'; // in principle, right after malloc d is uninitialised, printf() will go on 
                   // printing until it finds a '\0' in memory! so we initialize it here to 
                   // the empty string by settin its first character d[0].
                   //
        printf("d points to the string: '%s'      starting at %p \n\n",d,d); // empty
                                                                          //
        printf("sprintf(d,\"%%s\",\"Test3\")\n");
        sprintf(d,"%s","Test3"); // with sprintf() we modify d element-by-element, address stays the same
        printf("d points to the string: '%s' starting at %p (note the same address)\n\n",d,d);
    
        printf("d = \"Test3\"\n");
        d = "Test3";  // this way memory is allocated automatically as read-only
        // d[4]='X'; // this would segfault because d now is read-only
        printf("d points the string: '%s' starting at %p (note the change of address, now on the read-only text-segment)\n\n",d,d);


        printf("e = &a\n");
        e =  &a;
        printf("*e points to the string: '%s' starting at %p, which is the same  as a (%p) \n",*e,e, a);
        printf("Note the dereferencing operator * to obtain the content pointed by e\n\n");
        printf("e[0][4] = 'Y'\n");
        e[0][4]= 'Y' ;
        printf("e is 'writable' in the sense that it's just a pointer to a writable memory area\n\n");
        printf("f[0] = \"Test5\"\n");
        printf("f[9] = \"Test6\"\n");
        f[0] = "Test5";
        f[9] = "Test6";

        printf("f[0] points to the string: '%s' starting at %p\n",f[0],f[0]);
        printf("its elements '%c' '%c' '%c' .... are at offsets of  %ld bytes\n",f[0][0],f[0][1],f[0][2],
                                                                                          &f[0][1]-&f[0][0]);
        printf("f[9] points to the string: '%s' starting at %p\n\n",f[9],f[9]);
        printf("At this point, calling f[1] would most likely segfault, because we have not initialized it!\n\n");

        printf("We can reuse each of the 10 pointers f[0]...f[9] to point somewhere,\n");
        printf("for example to some dynamically allocated memory:\n");
        f[0] = (char*) malloc(10*sizeof(char));
        sprintf(f[0],"%s","Test7");
        printf("f[0] = (char*) malloc(10*sizeof(char))\n");
        printf("sprintf(f[0],\"%%s\",\"Test7\")\n");
        printf("f[0] points to the string: '%s' starting at %p\n\n",f[0],f[0]);

        //*g = "Test8"; // this would segfault because **g is not pointing to allocated memory yet.
        g = (char**) malloc(10 * sizeof(char*));
        *g = "Test8";
        printf("*g points to the string: '%s' starting at %p\n", *g, *g);
        printf("This is the same as &g[0][0] (%p), or, equivalently g[0] (%p), clear, uh?\n\n",&g[0][0],g[0]);

        printf("Since we have allocated space for 10 pointers, we can use also, e.g., the ninth:\n");
        g[0] = "Test9";
        g[9] = "TestA";
        printf("g[0] points to the string: '%s' starting at %p, which is the same as *g (%p)\n",g[0], g[0],*g);
        printf("g[9] points to the string: '%s' starting at %p, which is the same as *(g+9) (%p), thanks to pointer algebra!\n",g[9], g[9],*(g+9));
}
