import java.io.*;
import java.util.Scanner;

public class Lex{
	public static void main(String[] args){
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
				List list = new List();
				list.append(0);
				list.moveFront();
				/*for(int j=0;j<lines.length;j++){
					System.out.println(lines[j]);
				}*/
				for(int k=1; k<lines.length; k++){
					boolean high = true;
					//System.out.println("Placing "+lines[k]);
					list.moveFront();
					if(list.index()==(list.length()-1)){
						//System.out.println("Comparing "+lines[k]+" and "+lines[list.get()]);
						if(lines[k].compareTo(lines[list.get()])<0){
							high=false;
						}
					}
					while(high&&list.index()<(list.length()-1)){
						//System.out.println("Comparing "+lines[k]+" and "+lines[list.get()]);
						if(lines[k].compareTo(lines[list.get()])<=0){
							//System.out.println("Found place");
							high=false;
							break;
						}
						list.moveNext();
					}
					if(list.index()==(list.length()-1)){
						//System.out.println("Comparing "+lines[k]+" and "+lines[list.get()]);
						if(lines[k].compareTo(lines[list.get()])<0){
							high=false;
						}
					}
					if(high){
						list.append(k);
						//System.out.println("Inserted at the end");
					}else{
						list.insertBefore(k);
						//System.out.println("Inserted at position "+(list.index()-1));
					}
					
				}
				/*for(list.moveFront();list.index()>=0;list.moveNext()){
					System.out.println(list.get()+" "+lines[list.get()]);
				}*/
				for(list.moveFront();list.index()>=0;list.moveNext()){
					out.println(lines[list.get()]);
				}
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