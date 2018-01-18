using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MeshIOTester : MonoBehaviour {

    public Vector3[] vertexvector;
    float testx;
    // Use this for initialization
    void Start()
    {
        SkinnedMeshRenderer skinMesh = GetComponent<SkinnedMeshRenderer>();
        Mesh testMesh = skinMesh.sharedMesh;
        vertexvector = testMesh.vertices;
        testx = vertexvector[0].x;
        Debug.Log(testx);
    }

    // Update is called once per frame
    void Update()
    {

    }
}

