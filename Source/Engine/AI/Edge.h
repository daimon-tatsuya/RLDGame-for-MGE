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
//　　6   -  ・  -   2
//　　　  /　|   ＼
//    　 5      4　　3
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

//constexpr constant expression(定数式)の略
//https://qiita.com/saltheads/items/dd65935878a0901fe9e7

constexpr int EDGE_NUM = 8;// エッジの数　≒　方向の数


class Edge
{
private:

public:

	int destination_node_id = 0;// 接続先ノードID
	int origin_node_id = 0;		// 元ノードID
	float cost = 1.0f;			// コスト

private:

public:

	Edge() = default;
	~Edge() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="node">ノードID</param>
	/// <param name="destination">接続先</param>

	void Initialize(const int node, int& destination);
};
