import java.util.ArrayList;

public class Main
{
    public static void main(String[] args) 
    {
        ArrayList<DayofYearSet.DayofYear> arr1 = new ArrayList<DayofYearSet.DayofYear>();

        DayofYearSet.DayofYear obj1 = new DayofYearSet.DayofYear(24,6);
        arr1.add(obj1);
        DayofYearSet.DayofYear obj2 = new DayofYearSet.DayofYear(25,6);
        arr1.add(obj2);
        DayofYearSet.DayofYear obj3 = new DayofYearSet.DayofYear(1,4);
        arr1.add(obj3);
        DayofYearSet.DayofYear obj4 = new DayofYearSet.DayofYear(2,4);
        arr1.add(obj4);

        System.out.println("----------------------------------------");
        System.out.println("Set 1 Content");
        DayofYearSet mainObj1 = new DayofYearSet(arr1);
        /* 
        implementation of 'var' keyword:
        var mainObj1 = new DayofYearSet();
        var keyword is not available before Java SE 10
        */
        mainObj1.printSet();

        ArrayList<DayofYearSet.DayofYear> arr2 = new ArrayList<DayofYearSet.DayofYear>();

        DayofYearSet.DayofYear obj5 = new DayofYearSet.DayofYear(24,6);
        arr2.add(obj5);
        DayofYearSet.DayofYear obj6 = new DayofYearSet.DayofYear(1,1);
        arr2.add(obj6);
        DayofYearSet.DayofYear obj7 = new DayofYearSet.DayofYear(1,2);
        arr2.add(obj7);
        DayofYearSet.DayofYear obj8 = new DayofYearSet.DayofYear(1,3);
        arr2.add(obj8);

        System.out.println("----------------------------------------");
        System.out.println("Set 2 Content");
        DayofYearSet mainObj2 = new DayofYearSet(arr2);
        mainObj2.printSet();
      
        System.out.println("----------------------------------------");
        System.out.println("Union of Set 1 and Set 2");
        DayofYearSet mainObj3 = mainObj1.union(mainObj2);
        mainObj3.printSet();

        System.out.println("----------------------------------------");
        System.out.println("Intersection of Set 1 and Set 2");
        DayofYearSet mainObj4 = mainObj1.intersection(mainObj2);
        mainObj4.printSet();

        System.out.println("----------------------------------------");
        System.out.println("Difference of Set 1 and Set 2");
        DayofYearSet mainObj5 = mainObj1.difference(mainObj2);
        mainObj5.printSet();

        System.out.println("----------------------------------------");
        System.out.println("Complement of Set 1");
        DayofYearSet mainObj6 = mainObj1.complement();
        mainObj6.printSet();

        System.out.println("----------------------------------------");
        System.out.println("Set 1 Printing the Content using toString");
        System.out.println(mainObj1.toString());

        System.out.println("----------------------------------------");
        System.out.println("Adding 30.1 to Set 1");
        DayofYearSet.DayofYear obj9 = new DayofYearSet.DayofYear(30,1);
        mainObj1.add(obj9);
        mainObj1.printSet();

        System.out.println("----------------------------------------");
        System.out.println("Removing 1.4 to Set 1");
        DayofYearSet.DayofYear obj10 = new DayofYearSet.DayofYear(1,4);
        mainObj1.remove(obj10);
        mainObj1.printSet();

        System.out.println("----------------------------------------");
        System.out.println("Compare Set 1 and Set 2 using equals");
        boolean res = mainObj1.equals(mainObj2);
        System.out.println(res);

        System.out.println("----------------------------------------");
        System.out.println("Total Number of DayofYear Objects:");
        System.out.println(mainObj1.totalDayofYear());

        System.out.println("----------------------------------------");
        System.out.println("Write Set 1 to a text file called 'sets.txt'");
        mainObj1.writeFile();
        System.out.println("Write Set 2 to a text file called 'sets.txt'");
        mainObj2.writeFile();


        //De Morgan Rule
        System.out.println("----------------------------------------");
        System.out.println("De Morgan Rule");
        System.out.println("Set 1 Content");
        mainObj1.printSet();
        System.out.println("Set 2 Content");
        mainObj2.printSet();
        DayofYearSet mainObj7 = mainObj1.union(mainObj2); 
        DayofYearSet mainObj8 = mainObj7.complement();
        System.out.println("The result of !(Set 1 + Set 2):");
        mainObj8.printSet();
        DayofYearSet mainObj9 = mainObj1.complement();
        DayofYearSet mainObj10 = mainObj2.complement();
        DayofYearSet mainObj11 = mainObj9.intersection(mainObj10);
        System.out.println("The result of !Set 1 ^ !Set 2:");
        mainObj11.printSet();

        System.out.printf("The result of !(Set 1 + Set 2) == !Set 1 ^ !Set 2: ");
      
        res = mainObj8.equals(mainObj11);
        System.out.println(res);

        System.out.println("----------------------------------------");
    }
}