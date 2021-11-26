#pragma once

//**********************************************************
//
//		Edgeクラス
//
//**********************************************************



//0,  /*エッジの表示　上の方向*/
//1,  /*エッジの表示　右上の方向*/
//2,  /*エッジの表示　右の方向*/
//3,  /*エッジの表示　右下の方向*/
//4,  /*エッジの表示　下の方向*/
//5,  /*エッジの表示　左下の方向*/
//6,  /*エッジの表示　左の方向*/
//7,  /*エッジの表示　左上の方向*/
//　　　7     0      1
//  　　  ＼  |   /
//　　6    -  ・  -   2
//　　　  /　|   ＼
//    　 5      4　　 3
enum class EdgeDirection :int
{
	TopCenter = 0,
	TopRight,
	CenterRight,
	BottomRight,
	BottomCenter,
	BottomLeft,
	CenterLeft,
	TopLeft
};

const int EDGE_NUM = 8;//エッジの数　≒　方向の数


class Edge
{
private:

public:

	int distnation_node = 0;//接続先ノードID
	int origin_node = 0;//元ノードID
	float cost = 1.0f;  //コスト

private:

public:

	Edge() {}
	~Edge() {}

	//初期化
	void Initialize(int node, int& buffer);

};
