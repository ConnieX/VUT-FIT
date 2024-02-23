package vut.fit.ija.homework1.myMaps;

import vut.fit.ija.homework1.maps.Coordinate;

import java.util.Objects;

public class MyCoordinate implements Coordinate{
    int x;
    int y;
    public int getX(){
        return x;
    }

    public int getY(){
        return y;
    }

    public MyCoordinate(int x, int y){
        this.x = x;
        this.y = y;
    }

    @Override
    public boolean equals(Object o){
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        MyCoordinate that = (MyCoordinate) o;
        return x == that.x &&
                y == that.y;
    }

    @Override
    public int hashCode(){
        return Objects.hash (x, y);
    }

    public static MyCoordinate create(int x, int y) {
        if(x >= 0 && y >= 0) {
            return new MyCoordinate(x, y);
        }
        else{
            return null;
        }
    }
}
