package zombieSurvival;

public class Zombie {
	
	private int width, height;
	protected int speed, health;
	private float x, y;
	
	public Zombie(int x, int y) {
		this.x = x;
		this.y = y;
		width = 30;
		height = 30;
		health = 1;
		speed = 2;
	}
	
	public boolean collides(Projectile p) {
		for(float pixelX = x; pixelX <= x + width; pixelX++) {
			for(float pixelY = y; pixelY <= y + height; pixelY++) {
				if(Math.sqrt(Math.pow(pixelX - p.getX(), 2) + Math.pow(pixelY - p.getY(), 2)) <= p.getRadius()) {
					takeHit();
					return true;
				}
			}
		}
		return false;
	}
	
	public void handleCollision(Player p) {
		if(overlaps(p)) {
			p.takeHit();
		}
	}
	
	public boolean overlaps(Player p) {
		if (y + height < p.getY() || y > p.getY() + p.getHeight())
			return false;
		if (x + width < p.getX() || x > p.getX() + p.getWidth())
			return false;
		return true;		
	}
	
	public void takeHit() {
		health--;
	}
	
	public void moveToPlayer(Player p) {
		float xSpeed = Math.abs((float) (speed * Math.cos(Math.atan((p.getY() - y) / (p.getX() - x)))));
		float ySpeed = Math.abs((float) (speed * Math.sin(Math.atan((p.getY() - y) / (p.getX() - x)))));
		if(p.getX() >= x)
			x += xSpeed;
		else
			x -= xSpeed;
		if(p.getY() >= y)
			y += ySpeed;
		else
			y -= ySpeed;
		
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
}
