using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ExampleClass : MonoBehaviour {

    public Vector3[] newVertcies;
    public Vector3 testVertex;
    public Vector2[] newUV;
    public int[] newTriangles;

    public Mesh mm;

    void Start()
    {
        // Mesh mesh = new Mesh();
        //GetComponent<MeshFilter>().mesh = mesh;

        //컴포넌트 연습
        mm = new Mesh();
        mm = GetComponent<Mesh>();
        newVertcies = mm.vertices;
        

        Debug.Log(newVertcies);
        //mesh.Clear();

        //mesh.vertices = newVertcies;
        //mesh.uv = newUV;
        //mesh.triangles = newTriangles;
        
    }
    // Update is called once per frame
    void Update () {
        //Mesh mesh = GetComponent<MeshFilter>().mesh;
        //Vector3[] vertices = mesh.vertices;
        //Vector3[] normals = mesh.normals;
        //int i = 0;
        //while (i < vertices.Length)
        //{
        //    vertices[i] += normals[i] * Mathf.Sin(Time.time);
        //}
        //mesh.vertices = vertices;
	}
}
