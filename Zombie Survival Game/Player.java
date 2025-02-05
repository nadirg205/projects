package zombieSurvival;

public class Player {

	private int width, height, health, speed;
	private float x, y;
	public Player(int x, int y) {
		this.x = x;
		this.y = y;
		width = 30;
		height = 30;
		health = 1;
		speed = 3;
	}
	
	public void move(double angle) {
		x += speed * Math.cos(angle);
		y -= speed * Math.sin(angle);
	}
	
	public boolean collides(PowerUp p) {
		for(float pixelX = x; pixelX <= x + width; pixelX++) {
			for(float pixelY = y; pixelY <= y + height; pixelY++) {
				if(Math.sqrt(Math.pow(pixelX - p.getX(), 2) + Math.pow(pixelY - p.getY(), 2)) <= p.getRadius()) {
					return true;
				}
			}
		}
		return false;
	}
	
	public void takeHit() {
		health--;
	}
	
	public float getX() {
		return x;
	}
	
	public float getY() {
		return y;
	}
	
	public int getWidth() {
		return width;
	}
	
	public int getHeight() {
		return height;
	}
	public int getHealth() {
		return health;
	}
	
	public int getSpeed() {
		return speed;
	}
	
	public void increaseSpeed() {
		speed++;
	}
}
