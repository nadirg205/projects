package zombieSurvival;

public class PowerUp {
	private String type;
	private int x, y;
	private int radius;
	
	public PowerUp(String type, float x, float y) {
		this.type = type;
		this.x = (int) x;
		this.y = (int) y;
		radius = 6;
	}
	
	public int getRadius() {
		return radius;
	}
	
	public float getX() {
		return x;
	}
	
	public float getY() {
		return y;
	}

	public String getType() {
		return type;
	}
}
