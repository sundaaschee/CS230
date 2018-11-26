public class Duck{
    private final String name;
    public Duck(String name){
	this.name = name;
    }
    public String talk(){
	return "Duck " + name + " says Quack!";
    }
    public static void main(String[] args){
	Duck duck = new Duck("Joe");
	System.out.println(duck.talk());
    }
}
