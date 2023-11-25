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
// 	Sparse.java
// File Description:
// 	Main file for manipulating a Matrix ADT
//-----------------------------------------------------------------------------------------

import java.io.*;
import java.util.Scanner;

public class Sparse{
	public static void main(String args[]){
		if(args.length==2){
			String inFile = args[0];
			String outFile = args[1];
				try{
				Scanner in = new Scanner(new File(inFile));
				PrintWriter out = new PrintWriter(new FileWriter(outFile));
				int i = 0;
				while(in.hasNextLine()){
					i++;
					in.nextLine();
				}
				String[] lines = new String[i];
				in = new Scanner(new File(inFile));
				i=0;
				while(in.hasNextLine()){
					lines[i] = in.nextLine();
					i++;
				}
				String[] first = lines[0].split(" ");
				int n = Integer.parseInt(first[0]);
				int a = Integer.parseInt(first[1]);
				int b = Integer.parseInt(first[2]);
				//System.out.println("n="+n+", a="+a+", b="+b);
				Matrix A = new Matrix(n);
				Matrix B = new Matrix(n);
				int line = 2;
				int row;
				int column;
				double info;
				String[] split;
				for(i=0;i<a;i++){
					split = lines[line].split(" ");
					row = Integer.parseInt(split[0]);
					column = Integer.parseInt(split[1]);
					info = Double.parseDouble(split[2]);
					//System.out.println("inserting ("+row+","+column+","+info+")");
					A.changeEntry(row,column,info);
					line++;
				}
				line++;
				for(i=0;i<b;i++){
					split = lines[line].split(" ");
					row = Integer.parseInt(split[0]);
					column = Integer.parseInt(split[1]);
					info = Double.parseDouble(split[2]);
					B.changeEntry(row,column,info);
					line++;
				}
				out.println("A has "+A.getNNZ()+" non-zero entries:\n"+A);
				out.println("B has "+B.getNNZ()+" non-zero entries:\n"+B);
				out.println("(1.5)*A =\n"+A.scalarMult(1.5));
				out.println("A+B =\n"+A.add(B));
				out.println("A+A =\n"+A.add(A));
				out.println("B-A =\n"+B.sub(A));
				out.println("A-A =\n"+A.sub(A));
				out.println("Transpose(A) =\n"+A.transpose());
				out.println("A*B =\n"+A.mult(B));
				out.println("B*B =\n"+B.mult(B));
				in.close();
				out.close();
			} catch(IOException e){
				System.err.println("Could not open files for read/write");
				System.exit(1);
			}	
		} else{
			System.err.println("Incorrect number of arguments");
		}
	}
}
