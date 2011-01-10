/* Copyright (c) 2006-2007 Christopher J. W. Lloyd

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#import <Foundation/NSMutableArray_concrete.h>
#import <Foundation/NSRaise.h>
#import <Foundation/NSCoder.h>
#import <Foundation/NSRaiseException.h>

@implementation NSMutableArray_concrete

static inline NSUInteger roundCapacityUp(NSUInteger capacity){
   return (capacity<1)?1:capacity;
}

NSArray *NSMutableArray_concreteInit(NSMutableArray_concrete *self,id *objects,NSUInteger count,NSZone *zone) {
   NSUInteger i;

   self->_count=count;
   self->_capacity=roundCapacityUp(count);
   self->_objects=NSZoneMalloc(zone,sizeof(id)*self->_capacity);
   for(i=0;i<count;i++)
    self->_objects[i]=[objects[i] retain];

   return self;
}

NSArray *NSMutableArray_concreteInitWithCapacity(NSMutableArray_concrete *self,NSUInteger capacity,NSZone *zone) {
   self->_count=0;
   self->_capacity=roundCapacityUp(capacity);
   self->_objects=NSZoneMalloc(zone,sizeof(id)*self->_capacity);

   return self;
}

NSArray *NSMutableArray_concreteNew(NSZone *zone,id *objects,NSUInteger count) {
   NSMutableArray_concrete *self=NSAllocateObject([NSMutableArray_concrete class],0,zone);

   return NSMutableArray_concreteInit(self,objects,count,zone);
}

NSArray *NSMutableArray_concreteNewWithCapacity(NSZone *zone,NSUInteger capacity) {
   NSMutableArray_concrete *self=NSAllocateObject([NSMutableArray_concrete class],0,zone);

   return NSMutableArray_concreteInitWithCapacity(self,capacity,zone);
}

-init {
   return NSMutableArray_concreteInitWithCapacity(self,0,
     NSZoneFromPointer(self));
}

-initWithArray:(NSArray *)array {
   NSUInteger i,count=[array count];

   NSMutableArray_concreteInitWithCapacity(self,count,NSZoneFromPointer(self));
   self->_count=count;
   [array getObjects:_objects];
   for(i=0;i<count;i++)
    [_objects[i] retain];

   return self;
}

-initWithContentsOfFile:(NSString *)path {
   NSUnimplementedMethod();
   return self;
}

-initWithObjects:(id *)objects count:(NSUInteger)count {
   return NSMutableArray_concreteInit(self,objects,count,
     NSZoneFromPointer(self));
}

-initWithObjects:object,... {
   va_list  arguments;
   NSUInteger i,count;
   id      *objects;

   va_start(arguments,object);
   count=1;
   while(va_arg(arguments,id)!=nil)
    count++;
   va_end(arguments);

   objects=__builtin_alloca(sizeof(id)*count);

   va_start(arguments,object);
   objects[0]=object;
   for(i=1;i<count;i++)
    objects[i]=va_arg(arguments,id);
   va_end(arguments);

   return NSMutableArray_concreteInit(self,objects,count,NSZoneFromPointer(self));
}

-initWithCapacity:(NSUInteger)capacity {
   return NSMutableArray_concreteInitWithCapacity(self,capacity,
     NSZoneFromPointer(self));
}

-(void)dealloc {
   NSInteger count=_count;

   while(--count>=0)
    [_objects[count] release];

   NSZoneFree(NSZoneFromPointer(_objects),_objects);
   NSDeallocateObject(self);
   return;
   [super dealloc];
}

-(NSUInteger)count {
   return _count;
}

-objectAtIndex:(NSUInteger)index {
   if(index>=_count){
    NSRaiseException(NSRangeException,self,_cmd,@"index %d beyond count %d",index,[self count]);
    return nil;
   }

   return _objects[index];
}

-(void)addObject:object {
   if(object==nil){
    NSRaiseException(NSInvalidArgumentException,self,_cmd,@"nil object");
    return;
   }
   
   [object retain];

   _count++;
   if(_count>_capacity){
    _capacity=_count*2;
    _objects=NSZoneRealloc(NSZoneFromPointer(_objects),_objects,sizeof(id)*_capacity);
   }
   _objects[_count-1]=object;
}

-(void)replaceObjectAtIndex:(NSUInteger)index withObject:object {
   if(object==nil){
    NSRaiseException(NSInvalidArgumentException,self,_cmd,@"nil object");
    return;
   }
   
   if(index>=_count){
    NSRaiseException(NSRangeException,self,_cmd,@"index %d beyond count %d",index,[self count]);
    return;
   }

   [object retain];
   [_objects[index] release];
   _objects[index]=object;
}

-lastObject {
   if(_count==0)
    return nil;

   return _objects[_count-1];
}

-(void)insertObject:object atIndex:(NSUInteger)index {
   NSInteger c;

   if(object==nil){
    NSRaiseException(NSInvalidArgumentException,self,_cmd,@"nil object");
    return;
   }
   
   if(index>_count){
    NSRaiseException(NSRangeException,self,_cmd,@"index %d beyond count %d",index,[self count]);
    return;
   }

   _count++;
   if(_count>_capacity){
    _capacity=_count*2;
    _objects=NSZoneRealloc(NSZoneFromPointer(_objects),
      _objects,sizeof(id)*_capacity);
   }

   if(_count>1)
    for(c=_count-1;c>index && c>0;c--)
     _objects[c]=_objects[c-1];

   _objects[index]=[object retain];
}

static void removeObjectAtIndex(NSMutableArray_concrete *self,NSUInteger index) {
   NSUInteger i;
   id object;

   object=self->_objects[index];
   self->_count--;
   for(i=index;i<self->_count;i++)
    self->_objects[i]=self->_objects[i+1];

   [object release];

   if(self->_capacity>self->_count*2){
    self->_capacity=self->_count;
    self->_objects=NSZoneRealloc(NSZoneFromPointer(self->_objects),self->_objects,sizeof(id)*self->_capacity);
   }
}

-(void)removeObjectAtIndex:(NSUInteger)index {
   if(index>=_count){
    NSRaiseException(NSRangeException,self,_cmd,@"index %d beyond count %d",index,[self count]);
   }

  removeObjectAtIndex(self,index);
}

-(void)removeLastObject {
   if(_count==0){
    NSRaiseException(NSRangeException,self,_cmd,@"index %d beyond count %d",1,[self count]);
    return;
   }

   removeObjectAtIndex(self,_count-1);
}

-(void)removeAllObjects {
   NSUInteger i;
   
   for(i=0;i<_count;i++)
    [_objects[i] release];
    
   _count=0;
   if(self->_capacity>8){
    self->_capacity=8;
    self->_objects=NSZoneRealloc(NSZoneFromPointer(self->_objects),self->_objects,sizeof(id)*self->_capacity);
   }
}

-(void)getObjects:(id *)objects {
   NSUInteger i;

   for(i=0;i<_count;i++)
    objects[i]=_objects[i];
}

-(NSUInteger)indexOfObjectIdenticalTo:object {
   NSInteger i;

   for(i=0;i<self->_count;i++)
    if(self->_objects[i]==object)
     return i;

   return NSNotFound;
}

static inline NSUInteger indexOfObject(NSMutableArray_concrete *self,id object){
   NSUInteger i;

   for(i=0;i<self->_count;i++)
    if([self->_objects[i] isEqual:object])
     return i;

   return NSNotFound;
}

-(NSUInteger)indexOfObject:object {
   return indexOfObject(self,object);
}

-(BOOL)containsObject:object {
   return (indexOfObject(self,object)!=NSNotFound)?YES:NO;
}

-(void)makeObjectsPerformSelector:(SEL)selector {
	NSInteger i, count = [self count];
	
	for (i = 0; i < count; i++)
		[_objects[i] performSelector:selector];
}

// iterative mergesort based on http://www.inf.fh-flensburg.de/lang/algorithmen/sortieren/merge/mergiter.htm
-(void)sortUsingFunction:(NSInteger (*)(id, id, void *))compare context:(void *)context {
  NSInteger h, i, j, k, l, m, n = _count;
  id  A, *B = NSZoneMalloc(NULL,(n/2 + 1) * sizeof(id));

  for (h = 1; h < n; h += h)
  {
     for (m = n - 1 - h; m >= 0; m -= h + h)
     {
        l = m - h + 1;
        if (l < 0)
           l = 0;

        for (i = 0, j = l; j <= m; i++, j++)
           B[i] = _objects[j];

        for (i = 0, k = l; k < j && j <= m + h; k++)
        {
           A = _objects[j];
           if (compare(A, B[i], context) == NSOrderedDescending)
              _objects[k] = B[i++];
           else
           {
              _objects[k] = A;
              j++;
           }
        }

        while (k < j)
           _objects[k++] = B[i++];
     }
  }

  free(B);
}

-(NSUInteger)countByEnumeratingWithState:(NSFastEnumerationState *)state objects:(id *)stackbuf count:(NSUInteger)length;
{
   if(state->state>=_count)
    return 0;
   
   state->itemsPtr=_objects;
   state->state=_count;
   
   state->mutationsPtr=(unsigned long*)self;
	
   return _count;
}

@end
