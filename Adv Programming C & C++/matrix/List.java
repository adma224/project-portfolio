//-----------------------------------------------------------------------------------------
// Author: 
// 	Adrian Murillo
// CruzID: 
// 	amurill9
// ID#: 
// 	1573260
// Institution:
// 	UC Santa Cruz
// Course:
// 	CMPS 101
// File Name:
// 	List.java
// File Description:
// 	Doubly Linked List ADT functions
//-----------------------------------------------------------------------------------------
public class List{
	node head;
	node cursor;
	int index;
	public List(){
		head=null;
		cursor=null;
	}
	int length(){
		node rider = head;
		int length = 0;
		while(rider!=null){
			rider=rider.next;
			length++;
		}
		return length;
	}
	int index(){
		if(cursor!=null){
			return index;
		} else{
			return -1;
		}
	}
	Object front(){
		if(length()>0){
			return head.data;
		}else{
			return -1;
		}
	}
	Object back(){
		if(length()>0){
			node rider = head;
			while(rider.next!=null){
				rider=rider.next;
			}
			return rider.data;
		} else{
			return -1;
		}
	}
	Object get(){ // Returns cursor element. Pre: length()>0, index()>=0
		if(length()>0&&index()>=0){
			return cursor.data;
		} else{
			return -1;
		}
	}
	public boolean equals(List L){
		if(L.length()!=length()){
			return false;
		}
		if(head==L.head){
			return true;
		}
		node rider1 = L.head;
		node rider2 = this.head;
		while(rider1!=null&&rider2!=null){
			if(rider1.data!=rider2.data){
				return false;
			}
			rider1 = rider1.next;
			rider2 = rider2.next;
		}
		return true;
	}
	// Manipulation procedures
	void clear(){ // Resets this List to its original empty state.
		head=null;
		cursor=null;
		index=-1;
	}
	void moveFront(){ // If List is non-empty, places the cursor under the front element,
	 // otherwise does nothing.
		if(length()>0){
			cursor=head;
			index=0;
		}
	}
	void moveBack(){ // If List is non-empty, places the cursor under the back element,
	 // otherwise does nothing.
		if(length()>0){
			int i=0;
			node rider=head;
			while(rider.next!=null){
				rider=rider.next;
				i++;
			}
			index=i;
			cursor=rider;
		}
	}
	void movePrev(){ // If cursor is defined and not at front, moves cursor one step toward
	 // front of this List, if cursor is defined and at front, cursor becomes
	// undefined, if cursor is undefined does nothing.
		if(cursor!=null&&cursor!=head){
			cursor=cursor.prev;
			index--;
		} else{
			cursor=null;
			index=-1;
		}
	}
	void moveNext(){ // If cursor is defined and not at back, moves cursor one step toward
	 // back of this List, if cursor is defined and at back, cursor becomes
	// undefined, if cursor is undefined does nothing.
		if(index!=-1&&cursor!=null&&cursor.next!=null){
			cursor=cursor.next;
			index++;
		} else{
			cursor=null;
			index=-1;
		}
	}
	void prepend(Object data){ // Insert new element into this List. If List is non-empty,
	 // insertion takes place before front element.}
		if(length()>0){
			node n = new node(data);
			n.next=head;
			head.prev=n;
			head=n;	
			index++;
		} else{
			node n = new node(data);
			head = n;
		}
	}
	void append(Object data){ // Insert new element into this List. If List is non-empty,
	 // insertion takes place after back element.
		if(length()>0){
			node rider = head;
			while(rider.next!=null){
				rider=rider.next;
			}
			node n = new node(data);
			rider.next=n;
			n.prev=rider;
		} else{
			head=new node(data);
		}
	}
	void insertBefore(Object data){ // Insert new element before cursor.
	 // Pre: length()>0, index()>=0
		if(index()>=0&&length()>0){
			node n = new node(data);
			if(cursor==head){
				n.next=cursor;
				cursor.prev=n;
				head=n;
			} else{
				node temp = cursor.prev;
				temp.next=n;
				n.prev=temp;
				n.next=cursor;
				cursor.prev=n;
			}
			index++;
		}
	}
	void insertAfter(Object data){ // Inserts new element after cursor.
	 // Pre: length()>0, index()>=0
		if(index()>=0&&length()>0){
			node n = new node(data);
			if(cursor.next!=null){
				node temp = cursor.next;
				n.next=temp;
				temp.prev=n;
				cursor.next=n;
				n.prev=cursor;
			}else{
				cursor.next=n;
				n.prev=cursor;
			}
		}
	}
	void deleteFront(){ // Deletes the front element. Pre: length()>0
		if(length()>0){
			head=head.next;
			head.prev=null;
			if(index!=-1){
				index--;
			}
		}
	}
	void deleteBack(){ // Deletes the back element. Pre: length()>0
		if(length()>0){
			if(length()==1){
				head=null;
				cursor=null;
				index=-1;
			} else{
				node rider = head;
				while(rider.next!=null){
					rider=rider.next;
				}
				if(cursor==rider){
					cursor=null;
					index=-1;
				}
				node prev = rider.prev;
				prev.next = null;
				rider=null;
			}
		}
	}
	void delete(){ // Deletes cursor element, making cursor undefined.
	 // Pre: length()>0, index()>=0
	// Other methods
		if(length()>0&&index()>=0){
			if(length()==1){
				head=null;
			} else{
				if(cursor==head){
					head=head.next;
					head.prev=null;
				}else if(cursor.next==null){
					cursor=cursor.prev;
					cursor.next=null;
				}else{
					node prev=cursor.prev;
					node next=cursor.next;
					prev.next=next;
					next.prev=prev;
				}
			}
			cursor=null;
			index=-1;
		}
	}
	public String toString(){ // Overrides Object's toString method. Returns a String
	 // representation of this List consisting of a space
	 // separated sequence of integers, with front on left.
	    StringBuffer sb = new StringBuffer();
	    for(node n=head;n!=null;n=n.next){
	    	sb.append(n.data);
	        sb.append(" ");
		}
		sb.append("\n");
	    return new String(sb);
	}

	private class node{
		node prev;
		node next;
		Object data;
		private node(Object data){
			this.data=data;
			prev=null;
			next=null;
		}
	}
}