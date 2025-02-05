package zombieSurvival;

import java.awt.Point;

public class Projectile {
	private static int radius = 6;
	private static float speed = 5;
	private double xSpeed, ySpeed;
	
	private Point location, target;
	
	public Projectile(Point location, Point target) {
		this.location = location;
		this.target = target;
		xSpeed = Math.abs(speed * Math.cos(Math.atan((target.getY() - location.getY()) / (target.getX() - location.getX()))));
		if(location.getX() > target.getX())
			xSpeed = -xSpeed;
		ySpeed = Math.abs(speed * Math.sin(Math.atan((target.getY() - location.getY()) / (target.getX() - location.getX()))));
		if(location.getY() > target.getY())
			ySpeed = -ySpeed;
	}
	
	public void move() {
		location.setLocation(location.getX() + xSpeed, location.getY() + ySpeed);
	}
	
	public float getX() {
		return (float) location.getX();
	}
	
	public float getY() {
		return (float) location.getY();
	}
	
	public int getRadius() {
		return radius;
	}
}
