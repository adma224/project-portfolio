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
// 	Matrix.java
// File Description:
// 	Matrix ADT functions
//-----------------------------------------------------------------------------------------
public class Matrix{

	private class Entry{
		int column;
		int row;
		double info;
		Entry(int i, int j, double x){
			row=i;
			column=j;
			info=x;
		}
		public String toString(){
			return ("("+column+", "+info+")");
		}
	}
	// Constructor
	int n;
	List[] matrix;
	public Matrix(int n){
		if(n>=1){
			matrix = new List[n];
		}
		this.n=n;
	} // Makes a new n x n zero Matrix. pre: n>=1
	// Access functions
	int getSize(){
		return n;
	} // Returns n, the number of rows and columns of this Matrix
	int getNNZ(){
		int count=0;
		for(int i=0;i<n;i++){
			if(matrix[i]!=null){
				List list=matrix[i];
				if(list!=null){
					for(list.moveFront();list.index()!=-1;list.moveNext()){
						count++;
					}
				}
			}
		}
		return count;
	} // Returns the number of non-zero entries in this Matrix
	public boolean equals(Object x){
		Matrix m = (Matrix) x;
		if(getSize()!=m.getSize()){
			return false;
		}
		List list1;
		List list2;
		for(int i=0;i<n;i++){
			list1 = matrix[i];
			list2 = m.matrix[i];
			if(list1==null&&list2==null){
			} else if(list1!=null&&list2!=null){
				if(!list1.equals(list2)){
					return false;
				}
			} else{
				return false;
			}
		}
		return true;
	}
	// overrides Object's equals() method
	// Manipulation procedures
	void makeZero(){
		matrix = new List[n];
	} // sets this Matrix to the zero state
	Matrix copy(){
		Matrix m = new Matrix(n);
		for(int i=0;i<n;i++){
			List list = matrix[i];
			if(list!=null){
				m.matrix[i]=matrix[i];
			}
		}
		return m;
	}// returns a new Matrix having the same entries as this Matrix
	void changeEntry(int i, int j, double x){
		if(i<=getSize()&&j<=getSize()&&i>0&&j>0&&x!=0){
			Entry entry = new Entry(i,j,x);
			i--;
			List list = matrix[i];
			//System.out.println("Inserting new entry: ("+i+","+j+") "+x);
			if(list!=null){
				if(j==1){
					list.prepend(entry);
					return;
				}
				for(list.moveFront();list.index()!=-1;list.moveNext()){
					Entry e1 = (Entry) list.get();
					if(e1.column==j){
						list.insertBefore(entry);
						list.delete();
						return;
					}
					if(e1.column>j){
						list.insertBefore(entry);
						return;
					}
				}
				list.append(entry);
				
			} else{
				//System.out.println("List empty at "+i);
				matrix[i] = new List();
				matrix[i].prepend(entry);
			}
		}
	}
	 // changes ith row, jth column of this Matrix to x
	 // pre: 1<=i<=getSize(), 1<=j<=getSize()
	Matrix scalarMult(double x){
		Matrix result = new Matrix(n);
		for(int i=0;i<n;i++){
			List list = matrix[i];
			if(list!=null){
				for(list.moveFront();list.index()!=-1;list.moveNext()){
					Entry entry = (Entry) list.get();
					int row = entry.row;
					int column = entry.column;
					double info = x*entry.info;
					result.changeEntry(row,column,info);
				}
			}
		}
		return result;
	}
	 // returns a new Matrix that is the scalar product of this Matrix with x
	Matrix add(Matrix M){
		Matrix result= new Matrix(n);
		if(getSize()==M.getSize()){
			for(int i=0;i<n;i++){
				List list2 = M.matrix[i];
				List list1 = matrix[i];
				//System.out.println("    Line: "+(i+1));
				if(list2!=null&&list1!=null&&list1.length()>0&&list2.length()>0){
					int count = 1;
					list1.moveFront();
					list2.moveFront();
					Entry entry1 = (Entry) list1.get();
					Entry entry2 = (Entry) list2.get();
					double add;
					while(count<=n){
						add = 0;
						if(list1.index()==-1&&list2.index()==-1){
							break;
						}
						//System.out.println("Adding "+entry1+" "+entry2+" in line: "+(i+1)+", column: "+count);
						if(entry1.column==count){
							add += entry1.info;
							list1.moveNext();
							if(list1.index()!=-1){
								entry1 = (Entry) list1.get();
							}
						}
						if(entry2.column==count){
							add += entry2.info;
							if(list2!=list1){
								list2.moveNext();
							}
							if(list2.index()!=-1){
								entry2 = (Entry) list2.get();
							}
						}
						if(add!=0){
							//System.out.println("Added "+add+" in column: "+count);
							result.changeEntry(i+1,count,add);
						}
						count++;
					}
				} else if(list2!=null&&list1==null){
					result.matrix[i] = list2;
				} else if(list2==null&&list1!=null){
					result.matrix[i] = list1;
				}
			}
		}
		return result;
	}
	 // returns a new Matrix that is the sum of this Matrix with M
	 // pre: getSize()==M.getSize()
	Matrix sub(Matrix M){
		Matrix result= new Matrix(n);
		if(getSize()==M.getSize()){
			for(int i=0;i<n;i++){
				List list2 = M.matrix[i];
				List list1 = matrix[i];
				if(list2!=null&&list1!=null&&list1.length()>0&&list2.length()>0&&list1!=list2){
					int count = 1;
					list1.moveFront();
					list2.moveFront();
					Entry entry1 = (Entry) list1.get();
					Entry entry2 = (Entry) list2.get();
					double sub;
					while(count<=n){
						sub = 0;
						if(list1.index()==-1&&list2.index()==-1){
							break;
						}
						//System.out.println("Adding "+entry1+" "+entry2+" with count:"+count);
						if(entry1.column==count){
							sub += entry1.info;
							list1.moveNext();
							if(list1.index()!=-1){
								entry1 = (Entry) list1.get();
							}
						}
						if(entry2.column==count){
							sub -= entry2.info;
							if(list2!=list1){
								list2.moveNext();
							}
							if(list2.index()!=-1){
								entry2 = (Entry) list2.get();
							}
						}
						if(sub!=0){
							//System.out.println("Added "+add);
							result.changeEntry(i+1,count,sub);
						}
						count++;
					}
				} else if(list2!=null&&list1==null){
					for(list2.moveFront();list2.index()!=-1;list2.moveNext()){
						Entry entry = (Entry) list2.get();
						result.changeEntry(entry.row, entry.column, 0-entry.info);
					}
				} else if(list2==null&&list1!=null){
					result.matrix[i] = list1;
				}
			}
		}
		return result;
	}
	 // returns a new Matrix that is the difference of this Matrix with M
	 // pre: getSize()==M.getSize()
	Matrix transpose(){
		Matrix result = new Matrix(n);
		for(int i=0;i<n;i++){
			List list = matrix[i];
			if(list!=null){
				for(list.moveFront();list.index()!=-1;list.moveNext()){
					Entry entry = (Entry) list.get();
					result.changeEntry(entry.column,entry.row,entry.info);
				}
			}
		}
		return result;
	}
	 // returns a new Matrix that is the transpose of this Matrix
	Matrix mult(Matrix M){
		Matrix A = copy();
		Matrix B = M.transpose();
		Matrix result= new Matrix(A.getSize());
		if(getSize()==B.getSize()){
			for(int i=0;i<n;i++){
				List list2 = A.matrix[i];
				for(int j=0;j<n;j++){
					List list1 = B.matrix[j];
					double dot = dot(list1,list2);
					//System.out.println("Dot product for A"+i+" and B"+j+" = "+dot);
					if(dot!=0){
						result.changeEntry(i+1,j+1,dot);
					}
				}
			}
		}
		return result;
	}
	 // returns a new Matrix that is the product of this Matrix with M
	 // pre: getSize()==M.getSize()
	private static double dot(List P, List Q){
		if(P!=null&&Q!=null&&P.length()>0&&Q.length()>0&&P!=Q){
			Entry e1;
			Entry e2;
			double dot = 0;
			P.moveFront();
			Q.moveFront();
			while(P.index()!=-1&&Q.index()!=-1){
				e1 = (Entry) P.get();
				e2 = (Entry) Q.get();

				if(e1.column==e2.column){
					//System.out.println("  Adding "+e1+" and "+e2);
					dot += e1.info*e2.info;
					P.moveNext();
					Q.moveNext();
				} else{
					if(e1.column<e2.column){
						P.moveNext();
					}else{
						Q.moveNext();
					}
				}
			}
			return dot;
		} else if(P!=null&&P.length()>0&&P==Q){
			double dot = 0;
			Entry e;
			for(P.moveFront();P.index()!=-1;P.moveNext()){
				e = (Entry) P.get();
				dot += e.info*e.info;
			}
			return dot;
		} else{
			return 0;
		}
	}
	// Other functions*/
	public String toString(){
		StringBuffer sb = new StringBuffer();
		int i=0;
		for(i=0;i<n;i++){
		    List list = matrix[i];
		    if(list!=null){
		    	sb.append((i+1)+": ");
		    	sb.append(list);
		    }
		}
		return new String(sb);
	} // overrides Object's toString() method



}








