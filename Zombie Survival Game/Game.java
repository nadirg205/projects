package zombieSurvival;

import java.awt.Point;
import java.util.ArrayList;
import processing.core.PApplet;
import processing.core.PImage;

public class Game extends PApplet {
	
	private Player user;
	private ArrayList<Zombie> zombies;
	private ArrayList<Projectile> projectiles;
	private ArrayList<PowerUp> powerUps;
	private boolean upPressed, downPressed, leftPressed, rightPressed, reloading, dead;
	private float reloadTime; 
	private int reloadStart, bullets, maxBullets, nukes, zombiesToSpawn, baseReloadTime, startTime, timeSurvived;
	private boolean instructions = true;
	
	
	public static void main(String[] args) {
        PApplet.main("zombieSurvival.Game");
    }
	
	public void settings() {
		size(600,600);
	}
	
	public void setup() {
        user = new Player(300,300);
        projectiles = new ArrayList<Projectile>(1000);
        zombies = new ArrayList<Zombie>(1000);
        powerUps = new ArrayList<PowerUp>(1000);
        upPressed = false;
        downPressed = false;
        leftPressed = false;
        rightPressed = false;
        dead = false;
        reloading = false;
        baseReloadTime = 1500;
        bullets = 8;
        maxBullets = 8;
        nukes = 0;
        zombiesToSpawn = 3;
        startTime = millis();
    }
	
	public void draw() {
		if(instructions) {
			textAlign(CENTER);
			rectMode(CENTER);
			fill(0);
			background(255, 100, 100);
			textSize(40);
			text("Zombie Survival Game", 300, 100);
			
			textSize(28);
			text("Try to survive for as long as possible", 300,140);
			
			text("Controls", 150, 200);
			text("WASD: Movement", 150, 235);
			text("LMB: Shoot", 150, 270);
			text("RMB: Use Nuke", 150, 305);
			text("R: Reload", 150, 340);
			
			text("Powerups", 450, 200);
			text("Bullet Capacity ^", 450, 235);
			text("Reload Speed ^", 450, 270);
			text("Movement Speed ^", 450, 305);
			text("Nukes ^", 450, 340);

			fill(0, 96, 114);
			circle(340, 225, 6);
			fill(255, 165, 0);
			circle(347, 260, 6);
			fill(255, 255, 160);
			circle(325, 295, 6);
			fill(255, 191, 204);
			circle(390, 330, 6);
			
			fill(255);
			rect(300, 450, 150, 40);
			textSize(40);
			fill(0);
			text("START", 300, 462);
		}
		else if(!dead) {
			rectMode(CORNER);
			background(255, 100, 100);
			spawnZombies();
			handleReload();
			loadSprites();
			drawBulletsLeft();
			handleCollisions();
			moveSprites();
			drawTimer();
			if(user.getHealth() <= 0) {
				dead = true;
				timeSurvived = (millis() - startTime) / 1000;
			}
		}
		else {
			textAlign(CENTER);
			rectMode(CENTER);
			fill(255);
			background(0);
			textSize(40);
			text("GAME OVER", 300, 270);
	        textSize(24);
	        text("You survived " + timeSurvived + " seconds.", 300, 300);
	        rect(300, 340, 150, 40);
	        fill(0);
	        text("RETRY?", 300, 350);
		}
	}
	
	private void spawnZombies() {
		if((millis() - startTime) % 5000 >= 0 && (millis() - startTime) % 5000 <= 16.66) {
	        for(int i = 0; i < zombiesToSpawn; i++) {
	        	int x = 0, y = 0;
	        	if(Math.random() > .5) {
	        		if(Math.random() > .5)
	        			y = -30;
	        		else 
	        			y = 600;
        			x = (int) (Math.random() * 661) - 30;
	        	}
	        	else {
	        		if(Math.random() > .5)
	        			x = -30;
	        		else 
	        			x = 600;
        			y = (int) (Math.random() * 661) - 30;
	        	}
	        	double rand = Math.random();
	        	if(rand >= 0.5)
	        		zombies.add(new Zombie(x,y));
	        	else if(rand >= .25)
	        		zombies.add(new TankZombie(x,y));
	        	else if(rand >= .1)
	        		zombies.add(new FastZombie(x,y));
	        	else
	        		zombies.add(new SuperZombie(x,y));
	        }
			zombiesToSpawn++;
		}
	}

	private void handleReload() {
		if(reloading) {
			if(reloadStart + reloadTime < millis()) {
				reloading = false;
				bullets = maxBullets;
			}
		}
	}

	private void drawBulletsLeft() {
        if(nukes > 0)
	        fill(255, 191, 204);
        else
        	fill(200, 200, 200);
        noStroke();
        if(reloading)
        	rect(user.getX(), user.getY() - 10, user.getWidth() * (float) (millis() - reloadStart) / reloadTime, 5);
        else
        	rect(user.getX(), user.getY() - 10, user.getWidth() * (float) bullets / maxBullets, 5);
		noFill();
		stroke(0);
        strokeWeight(1);
        rect(user.getX(), user.getY() - 10, user.getWidth(), 5);
	}

	private void killAllZombies() {
		for(int i = 0; i < zombies.size(); i++) {
			Zombie z = zombies.get(i);
			trySpawnPowerUp((int) (z.getX() + z.getWidth()), (int) (z.getY() + z.getHeight()));
		}
		zombies = new ArrayList<Zombie>(1000);
	}
	
	private void trySpawnPowerUp(int x, int y) {
		if(Math.random() >= .75) {
			String type = "";
			double rand = Math.random();
			if(rand >= .9)
				type = "nuke";
			else if(rand >= .8)
				type = "speed";
			else if(rand >= .3) 
				type = "bullets";
			else
				type = "reload";
			powerUps.add(new PowerUp(type, x, y));
		}
	}
	
	private void drawTimer() {
		fill(0);
		textSize(12);
		text((millis() - startTime) / 1000, 10, 15);
	}

	public void moveSprites() {
		movePlayer();
		for(Zombie z : zombies) {
			z.moveToPlayer(user);
		}
		int projectile = 0;
		while(projectile < projectiles.size()) {
			Projectile p = projectiles.get(projectile);
			p.move();
			if(p.getX() < 0 || p.getX() > width || p.getY() < 0 || p.getY() > height) {
				projectiles.remove(projectile);
			}
			else
				projectile++;
		}
	}
	
	public void handleCollisions() {
		int zombie = 0;		
		while(zombie < zombies.size()) {
			Zombie z = zombies.get(zombie);
			int projectile = 0;
			while(projectile < projectiles.size()) {
				Projectile p = projectiles.get(projectile);
				if(z.collides(p)) {
					projectiles.remove(projectile);
				}
				else
					projectile++;
			}
			if(z.getHealth() <= 0) {
				trySpawnPowerUp((int) (z.getX() + z.getWidth()), (int) (z.getY() + z.getHeight()));
				zombies.remove(zombie);
			}
			else {
				z.handleCollision(user);
				zombie++;
			}	
		}
		int powerUp = 0;
		while(powerUp < powerUps.size()) {
			PowerUp p = powerUps.get(powerUp);
			if(user.collides(p)) {
				powerUps.remove(powerUp);
				if(p.getType().equals("bullets"))
					maxBullets++;
				if(p.getType().equals("nuke"))
			        nukes++;
				if(p.getType().equals("speed") && user.getSpeed() < 6)
					user.increaseSpeed();
				if(p.getType().equals("reload") && baseReloadTime > 100)
					baseReloadTime -= 100;
			}
			else
				powerUp++;
		}
	}
	
	public void loadSprites() {
        stroke(0);
		strokeWeight(1);
		for(Zombie z : zombies) {
			fill(0,255,0);
			rect(z.getX(), z.getY(), z.getWidth(), z.getHeight());
		}
		for(Projectile p : projectiles) {
			fill(0);
			circle(p.getX(), p.getY(), p.getRadius());
		}
		for(PowerUp p : powerUps) {
			if(p.getType().equals("bullets"))
				fill(0, 96, 114); //blue
			if(p.getType().equals("nuke"))
		        fill(255, 191, 204); // pink
			if(p.getType().equals("speed"))
				fill(255,255, 160); // yellow
			if(p.getType().equals("reload"))
				fill(255, 165, 0); //orange
			circle(p.getX(), p.getY(), p.getRadius());
		}
		fill(255, 0, 0);
		rect(user.getX(), user.getY(), user.getWidth(), user.getHeight());
	}
	
	public void movePlayer() {
		if(rightPressed && user.getX() <= width - user.getWidth()) 
			user.move(0);
		if(leftPressed && user.getX() >= 0) 
			user.move(Math.PI);
		if(upPressed && user.getY() >= 0) 
			user.move(Math.PI / 2);
		if(downPressed && user.getY() <= height - user.getHeight())
			user.move(-Math.PI / 2);
	}
	
	public void mousePressed() {
		if(instructions) {
			if(mouseX >= 225 && mouseX <= 375 && mouseY >= 430 && mouseY <= 470) {
				setup();
				instructions = false;
			}
			rect(300, 450, 150, 40);
		}
		else if(!dead) {
			if(mouseButton == LEFT && bullets > 0 && !reloading) {
				Point location = new Point();
				location.setLocation(user.getX() + user.getWidth() / 2, user.getY() + user.getHeight() / 2);
				Point target = new Point(mouseX, mouseY);
				projectiles.add(new Projectile(location, target));
				bullets--;
			}
			else if(mouseButton == LEFT && bullets == 0 && !reloading) {
				reload();
			}
			else if(mouseButton == RIGHT && nukes > 0) {
				killAllZombies();
				nukes--;
			}
		}
		else {
			if(mouseX >= 225 && mouseX <= 375 && mouseY >= 330 && mouseY <= 370) {
				setup();
			}
		}
	}
	
	public void keyPressed() {
		if(key != CODED) {
			if(key == 'w' || key == 'W') {
				upPressed = true;
			}
			else if(key == 's' || key == 'S') {
				downPressed = true;
			}
			else if(key == 'a' || key == 'A') {
				leftPressed = true;
			}
			else if(key == 'd' || key == 'D') {
				rightPressed = true;
			}
			else if((key == 'r' || key == 'R') && !reloading) {
				reload();
			}
		}
	}
	
	private void reload() {
		reloading = true;
		reloadStart = millis();
		reloadTime = (float) baseReloadTime * (maxBullets - bullets) / maxBullets;
	}

	public void keyReleased() {
		if(key != CODED) {
			if(key == 'w' || key == 'W') {
				upPressed = false;
			}
			else if(key == 's' || key == 'S') {
				downPressed = false;
			}else if(key == 'a' || key == 'A') {
				leftPressed = false;
			}
			else if(key == 'd' || key == 'D') {
				rightPressed = false;
			}
		}
	}
}
