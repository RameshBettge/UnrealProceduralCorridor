using System.Collections.Generic;
using UnityEngine;

public class BSPTree
{
    public BSPNode root;
    public List<BSPNode> allNodes = new List<BSPNode>(32);
    List<List<BSPNode>> nodeLists = new List<List<BSPNode>>();
    int minNodeWidth;
    int minNodeHeight;
    int numberOfCorridors;
    enum Farthest { Top, Right, Bottom, Left, }

    List<BSPNode> leafs = new List<BSPNode>(32);
    public List<BSPNode> Leafs
    {
        get { return leafs; }
    }

    public List<BSPNode>[] levels;

    public BSPTree(int width, int height, int minNodeWidth, int minNodeHeight)
    {
        bool doSplitVertically = width >= height;
        this.minNodeWidth = minNodeWidth;
        this.minNodeHeight = minNodeHeight;
        root = new BSPNode(null, new Rect(0f, 0f, width, height), doSplitVertically);
        Split(root);
    }

    public void Split(BSPNode node)
    {
        allNodes.Add(node);
        bool vertical = node.doSplitVertically;
        bool doNextSplitVertical = !vertical;
        bool bigEnough = node.doSplitVertically ? node.rect.width > minNodeWidth : node.rect.height > minNodeHeight;

        if (bigEnough)
        {
            float percent = Random.Range(0.3f, 0.7f);
            BSPNode[] currentChildren = node.SplitNode(percent);

            Split(currentChildren[0]);
            Split(currentChildren[1]);
        }
        else
        {
            leafs.Add(node);
        }
    }

    public void CreateNodeLists()
    {
        for (int i = 0; i < allNodes.Count; i++)
        {
            if (nodeLists.Count <= allNodes[i].Level)
            {
                nodeLists.Add(new List<BSPNode>());
            }
            nodeLists[allNodes[i].Level].Add(allNodes[i]);
        }
    }

    public void GenerateRooms()
    {
        for (int i = 0; i < leafs.Count; i++)
        {
            Rect leafRect = leafs[i].rect;
            float newWidth = leafRect.width * Random.Range(0.6f, 0.8f);
            float newHeight = leafRect.height * Random.Range(0.6f, 0.8f);
            float xMargin = leafRect.width - newWidth;
            xMargin *= Random.Range(0.2f, 0.6f);
            float yMargin = leafRect.height - newHeight;
            yMargin *= Random.Range(0.2f, 0.6f);
            float quantizedX = Mathf.Ceil((int)leafRect.x + xMargin);
            float quantizedY = Mathf.Ceil((int)leafRect.y + yMargin);
            float quantizedWidth = Mathf.Floor(newWidth);
            float quantizedHeight = Mathf.Floor(newHeight);
            leafs[i].room = new Rect(quantizedX, quantizedY, quantizedWidth, quantizedHeight);
            leafs[i].tileType = TileType.Room;
        }
    }

    public void GenerateCorridors()
    {
        List<BSPNode> candidates = new List<BSPNode>(16);
        for (int i = nodeLists.Count - 1; i > 0; i--)
        {
            for (int j = 0; j < nodeLists[i].Count; j++)
            {
                GenerateCorridor(nodeLists[i][j], candidates);
            }
        }
        Debug.Log("number of corridors: " + numberOfCorridors.ToString());
    }

    void GenerateCorridor(BSPNode node, List<BSPNode> candidates)
    {
        BSPNode sibling = node.sibling;
        bool isLeft = node.isResultOfVerticalSplit && node.rect.x < sibling.rect.x;
        bool isLow = !node.isResultOfVerticalSplit && node.rect.y < sibling.rect.y;
        if (!isLeft && !isLow) { return; }
        candidates.Clear();
        candidates.Add(node);
        node.FillListWithChildren(candidates);
        BSPNode bestNode = isLeft ? FindFarthestNode(Farthest.Right, candidates) : FindFarthestNode(Farthest.Top, candidates);
        candidates.Clear();
        candidates.Add(sibling);
        sibling.FillListWithChildren(candidates);
        List<BSPNode> overlapping = FindOverlapping(bestNode, candidates, !isLeft);
        if (overlapping.Count == 0) { return; }
        BSPNode bestSibling = isLeft ? FindFarthestNode(Farthest.Left, overlapping) : FindFarthestNode(Farthest.Bottom, overlapping);
        int x, y, width, height;
        if (isLeft)
        {
            x = (int)bestNode.room.xMax;
            y = (int)(Mathf.Max(bestNode.room.y, bestSibling.room.y) + halfOverlap(bestNode.room, bestSibling.room, false)) - 1;
            width = (int)bestSibling.room.x - x;
            height = 3;
        }
        else
        {
            x = (int)(Mathf.Max(bestNode.room.x, bestSibling.room.x) + halfOverlap(bestNode.room, bestSibling.room, true)) - 1; ;
            y = (int)bestNode.room.yMax;
            width = 3;
            height = (int)bestSibling.room.y - y;
        }
        numberOfCorridors++;
        node.parent.room = new Rect(x, y, width, height);
        node.parent.tileType = TileType.Corridor;
    }

    BSPNode FindFarthestNode(Farthest farthest, List<BSPNode> candidates)
    {
        BSPNode bestNode = candidates[0];
        for (int i = 1; i < candidates.Count; i++)
        {
            switch (farthest)
            {
                case Farthest.Top:
                    bestNode = candidates[i].room.yMax > bestNode.room.yMax ? candidates[i] : bestNode;
                    break;
                case Farthest.Right:
                    bestNode = candidates[i].room.xMax > bestNode.room.xMax ? candidates[i] : bestNode;
                    break;
                case Farthest.Bottom:
                    bestNode = candidates[i].room.y < bestNode.room.y ? candidates[i] : bestNode;
                    break;
                case Farthest.Left:
                    bestNode = candidates[i].room.x < bestNode.room.x ? candidates[i] : bestNode;
                    break;
                default:
                    break;
            }
        }
        return bestNode;
    }

    List<BSPNode> FindOverlapping(BSPNode node, List<BSPNode> candidates, bool xDim)
    {
        List<BSPNode> overlapping = new List<BSPNode>();
        for (int i = 0; i < candidates.Count; i++)
        {
            if (halfOverlap(node.room, candidates[i].room, xDim) > 1.5f)
            {
                overlapping.Add(candidates[i]);
            }
        }
        return overlapping;
    }

    float halfOverlap(Rect rect1, Rect rect2, bool xDim) // TODO Overlap draus machen
    {
        float val1 = xDim ? Mathf.Min(rect1.xMax, rect2.xMax) : Mathf.Min(rect1.yMax, rect2.yMax);
        float val2 = xDim ? Mathf.Max(rect1.xMin, rect2.xMin) : Mathf.Max(rect1.yMin, rect2.yMin);
        return (val1 - val2) * 0.5f;
    }
}
