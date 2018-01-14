using System.Collections;
using System.Collections.Generic;
using UnityEngine;
//입출력 추가 코드
using System.IO;
using System.Text;

namespace readwriteapp
{ 
    public class MeshTester : MonoBehaviour
    {
        public Vector3[] vertexvector;
        public Vector3[] tmpVertexvector;
        public Vector3[] normalvector;
        float testx;
        float[] tvec;
        public GameObject SpiderModel;
        //GameObject tes
        // Use this for initialization
        void Start()
        {
            SkinnedMeshRenderer skinMesh = GetComponent<SkinnedMeshRenderer>();
            Mesh testMesh = skinMesh.sharedMesh;
            
            vertexvector = testMesh.vertices;
            testx = vertexvector[0].x;
            Debug.Log(testx);
            //Int64 x;
            try
            {
                //Open the File 
                StreamWriter sw = new StreamWriter("C:\\Users\\LEE JUNG UK\\Desktop\\Team MT\\ImportText.txt"); //절대 경로 

                tvec = new float[3];
                //Write a line of text

                //FrameName 뽑기 (프레임 이름 ?)
                //sw.Write("FrameName: ");
                //// 하드 코딩하는게 마음에 들지 않음(나중에 어떻게 바꿔줘야할까) 
                //// 아마 네임 컨포넌트 받는게 있을텐데 지금은 모르겠음
                //sw.WriteLine("spider");
                //sw.Write("           "); //들여쓰기 
                //sw.Write("Transform: ");
                ////트랜스폼 행렬 뽑기
                ////transform(Framelocalx,y,z,/FramelocalPositionx,y,z,Quternion,Localscale,Scale)
                //Transform transformmatrix = GetComponent<Transform>();
                //sw.Write(transformmatrix.localPosition.x); sw.Write(" "); // frame local x
                //sw.Write(transformmatrix.localPosition.y); sw.Write(" "); // frame local y
                //sw.Write(transformmatrix.localPosition.z); sw.Write(" "); // frame local z

                //sw.Write(transformmatrix.localRotation.x); sw.Write(" "); // frame local rotation x
                //sw.Write(transformmatrix.localRotation.y); sw.Write(" "); // frame local rotation y
                //sw.Write(transformmatrix.localRotation.z); sw.Write(" "); // frame local rotation z

                //sw.Write(transformmatrix.rotation.x); sw.Write(" "); // frame quaternition rotation x
                //sw.Write(transformmatrix.rotation.y); sw.Write(" "); // frame quaternition rotation y
                //sw.Write(transformmatrix.rotation.z); sw.Write(" "); // frame quaternition rotation z
                //sw.Write(transformmatrix.rotation.w); sw.Write(" "); // frame quaternition rotation w

                //sw.Write(transformmatrix.localScale.x); sw.Write(" "); // frame local scale x
                //sw.Write(transformmatrix.localScale.y); sw.Write(" "); // frame local scale y
                //sw.Write(transformmatrix.localScale.z); sw.Write(" "); // frame local scale z

                //sw.Write(transformmatrix.lossyScale.x); sw.Write(" "); // scale x
                //sw.Write(transformmatrix.lossyScale.y); sw.Write(" "); // scale y
                //sw.Write(transformmatrix.lossyScale.z); sw.Write(" "); // scale z

                //sw.WriteLine(); // 줄바꿈

                //sw.Write("           "); //들여쓰기 
                //sw.Write("TransformMatrix: ");  //root transform matrix -확실치 않음 테스팅 후 사용

                //sw.Write(transformmatrix.localToWorldMatrix.m00); sw.Write(" "); // torootTransform.11
                //sw.Write(transformmatrix.localToWorldMatrix.m01); sw.Write(" "); // torootTransform.12
                //sw.Write(transformmatrix.localToWorldMatrix.m02); sw.Write(" "); // torootTransform.13
                //sw.Write(transformmatrix.localToWorldMatrix.m03); sw.Write(" "); // torootTransform.14

                //sw.Write(transformmatrix.localToWorldMatrix.m10); sw.Write(" "); // torootTransform.21
                //sw.Write(transformmatrix.localToWorldMatrix.m11); sw.Write(" "); // torootTransform.22
                //sw.Write(transformmatrix.localToWorldMatrix.m12); sw.Write(" "); // torootTransform.23
                //sw.Write(transformmatrix.localToWorldMatrix.m13); sw.Write(" "); // torootTransform.24

                //sw.Write(transformmatrix.localToWorldMatrix.m20); sw.Write(" "); // torootTransform.31
                //sw.Write(transformmatrix.localToWorldMatrix.m21); sw.Write(" "); // torootTransform.32
                //sw.Write(transformmatrix.localToWorldMatrix.m22); sw.Write(" "); // torootTransform.33
                //sw.Write(transformmatrix.localToWorldMatrix.m23); sw.Write(" "); // torootTransform.34

                //sw.Write(transformmatrix.localToWorldMatrix.m30); sw.Write(" "); // torootTransform.41
                //sw.Write(transformmatrix.localToWorldMatrix.m31); sw.Write(" "); // torootTransform.42
                //sw.Write(transformmatrix.localToWorldMatrix.m32); sw.Write(" "); // torootTransform.43
                //sw.Write(transformmatrix.localToWorldMatrix.m33); sw.Write(" "); // torootTransform.44

                //sw.WriteLine(); // 줄바꿈

                ////toparentmatrix 루트 매트릭스와 부모 매트릭스 똑같아서 일단 똑같이 씀 유니티에서 어떻게된지 잘 모르겠음.
                //sw.Write("           "); //들여쓰기 
                //sw.Write(transformmatrix.localToWorldMatrix.m00); sw.Write(" "); // torootTransform.11
                //sw.Write(transformmatrix.localToWorldMatrix.m01); sw.Write(" "); // torootTransform.12
                //sw.Write(transformmatrix.localToWorldMatrix.m02); sw.Write(" "); // torootTransform.13
                //sw.Write(transformmatrix.localToWorldMatrix.m03); sw.Write(" "); // torootTransform.14

                //sw.Write(transformmatrix.localToWorldMatrix.m10); sw.Write(" "); // torootTransform.21
                //sw.Write(transformmatrix.localToWorldMatrix.m11); sw.Write(" "); // torootTransform.22
                //sw.Write(transformmatrix.localToWorldMatrix.m12); sw.Write(" "); // torootTransform.23
                //sw.Write(transformmatrix.localToWorldMatrix.m13); sw.Write(" "); // torootTransform.24

                //sw.Write(transformmatrix.localToWorldMatrix.m20); sw.Write(" "); // torootTransform.31
                //sw.Write(transformmatrix.localToWorldMatrix.m21); sw.Write(" "); // torootTransform.32
                //sw.Write(transformmatrix.localToWorldMatrix.m22); sw.Write(" "); // torootTransform.33
                //sw.Write(transformmatrix.localToWorldMatrix.m23); sw.Write(" "); // torootTransform.34

                //sw.Write(transformmatrix.localToWorldMatrix.m30); sw.Write(" "); // torootTransform.41
                //sw.Write(transformmatrix.localToWorldMatrix.m31); sw.Write(" "); // torootTransform.42
                //sw.Write(transformmatrix.localToWorldMatrix.m32); sw.Write(" "); // torootTransform.43
                //sw.Write(transformmatrix.localToWorldMatrix.m33); sw.Write(" "); // torootTransform.44

                //sw.WriteLine(); // 줄바꿈
                //sw.Write("           "); //들여쓰기 

                ////자식 갯수
                //sw.Write("Children: ");
                //sw.Write(testMesh.subMeshCount);     
                //sw.WriteLine();

                //자식 뽑기 (유니티의 모든 게임오브젝트는 fransform를가지고 있으므로 위와 같이 탐색한다. )
                //아래 코드를 이용하여 위에 하드코딩한 내용들을 일반화 한다. 
                //var transformList = SpiderModel.GetComponentInChildren<Transform>();
                //if (transformList != null)
                //{
                //    foreach (var child in transformList)
                //    {
                //        sw.Write(child.ToString());
                //    }
                //}
                int loopcnt = 0; 
                var transformList1 = SpiderModel.GetComponentInChildren<Transform>().GetComponentsInChildren<Transform>();
                if (transformList1 != null)
                {
                    foreach (var child in transformList1)
                    {

                        for (int i = 0; i < loopcnt; ++i)
                        {
                            sw.Write("           "); //들여쓰기 
                        }
                        sw.Write("FrameName: "); sw.WriteLine(child.name);
                        for (int i = 0; i < loopcnt; ++i)
                        {
                            sw.Write("           "); //들여쓰기 
                        }
                        sw.Write("Transform: ");

                        sw.Write(child.localPosition.x); sw.Write(" "); // frame local x
                        sw.Write(child.localPosition.y); sw.Write(" "); // frame local y
                        sw.Write(child.localPosition.z); sw.Write(" "); // frame local z

                        sw.Write(child.localRotation.x); sw.Write(" "); // frame local rotation x
                        sw.Write(child.localRotation.y); sw.Write(" "); // frame local rotation y
                        sw.Write(child.localRotation.z); sw.Write(" "); // frame local rotation z

                        sw.Write(child.rotation.x); sw.Write(" "); // frame quaternition rotation x
                        sw.Write(child.rotation.y); sw.Write(" "); // frame quaternition rotation y
                        sw.Write(child.rotation.z); sw.Write(" "); // frame quaternition rotation z
                        sw.Write(child.rotation.w); sw.Write(" "); // frame quaternition rotation w

                        sw.Write(child.localScale.x); sw.Write(" "); // frame local scale x
                        sw.Write(child.localScale.y); sw.Write(" "); // frame local scale y
                        sw.Write(child.localScale.z); sw.Write(" "); // frame local scale z

                        sw.Write(child.lossyScale.x); sw.Write(" "); // scale x
                        sw.Write(child.lossyScale.y); sw.Write(" "); // scale y
                        sw.Write(child.lossyScale.z); sw.Write(" "); // scale z

                        sw.WriteLine(); // 줄바꿈

                        for (int i = 0; i < loopcnt; ++i)
                        {
                            sw.Write("           "); //들여쓰기 
                        }

                        sw.Write("TransformMatrix: ");  //root transform matrix -확실치 않음 테스팅 후 사용

                        sw.Write(child.localToWorldMatrix.m00); sw.Write(" "); // torootTransform.11
                        sw.Write(child.localToWorldMatrix.m01); sw.Write(" "); // torootTransform.12
                        sw.Write(child.localToWorldMatrix.m02); sw.Write(" "); // torootTransform.13
                        sw.Write(child.localToWorldMatrix.m03); sw.Write(" "); // torootTransform.14

                        sw.Write(child.localToWorldMatrix.m10); sw.Write(" "); // torootTransform.21
                        sw.Write(child.localToWorldMatrix.m11); sw.Write(" "); // torootTransform.22
                        sw.Write(child.localToWorldMatrix.m12); sw.Write(" "); // torootTransform.23
                        sw.Write(child.localToWorldMatrix.m13); sw.Write(" "); // torootTransform.24

                        sw.Write(child.localToWorldMatrix.m20); sw.Write(" "); // torootTransform.31
                        sw.Write(child.localToWorldMatrix.m21); sw.Write(" "); // torootTransform.32
                        sw.Write(child.localToWorldMatrix.m22); sw.Write(" "); // torootTransform.33
                        sw.Write(child.localToWorldMatrix.m23); sw.Write(" "); // torootTransform.34

                        sw.Write(child.localToWorldMatrix.m30); sw.Write(" "); // torootTransform.41
                        sw.Write(child.localToWorldMatrix.m31); sw.Write(" "); // torootTransform.42
                        sw.Write(child.localToWorldMatrix.m32); sw.Write(" "); // torootTransform.43
                        sw.Write(child.localToWorldMatrix.m33); sw.Write(" "); // torootTransform.44

                        sw.WriteLine(); // 줄바꿈

                        //toparentmatrix 루트 매트릭스와 부모 매트릭스 똑같아서 일단 똑같이 씀 유니티에서 어떻게된지 잘 모르겠음.
                        for (int i = 0; i < loopcnt; ++i)
                        {
                            sw.Write("           "); //들여쓰기 
                        }
                        sw.Write(child.localToWorldMatrix.m00); sw.Write(" "); // torootTransform.11
                        sw.Write(child.localToWorldMatrix.m01); sw.Write(" "); // torootTransform.12
                        sw.Write(child.localToWorldMatrix.m02); sw.Write(" "); // torootTransform.13
                        sw.Write(child.localToWorldMatrix.m03); sw.Write(" "); // torootTransform.14

                        sw.Write(child.localToWorldMatrix.m10); sw.Write(" "); // torootTransform.21
                        sw.Write(child.localToWorldMatrix.m11); sw.Write(" "); // torootTransform.22
                        sw.Write(child.localToWorldMatrix.m12); sw.Write(" "); // torootTransform.23
                        sw.Write(child.localToWorldMatrix.m13); sw.Write(" "); // torootTransform.24

                        sw.Write(child.localToWorldMatrix.m20); sw.Write(" "); // torootTransform.31
                        sw.Write(child.localToWorldMatrix.m21); sw.Write(" "); // torootTransform.32
                        sw.Write(child.localToWorldMatrix.m22); sw.Write(" "); // torootTransform.33
                        sw.Write(child.localToWorldMatrix.m23); sw.Write(" "); // torootTransform.34

                        sw.Write(child.localToWorldMatrix.m30); sw.Write(" "); // torootTransform.41
                        sw.Write(child.localToWorldMatrix.m31); sw.Write(" "); // torootTransform.42
                        sw.Write(child.localToWorldMatrix.m32); sw.Write(" "); // torootTransform.43
                        sw.Write(child.localToWorldMatrix.m33); sw.Write(" "); // torootTransform.44

                        
                        sw.WriteLine(); // 줄바꿈
                        for (int i = 0; i < loopcnt; ++i)
                        {
                            sw.Write("           "); //들여쓰기 
                        }

                        if (child.childCount != 0)
                        {
                            sw.Write("Child: ");
                            sw.WriteLine(child.childCount);
                            for (int i = 0; i < loopcnt; ++i)
                            {
                                sw.Write("           "); //들여쓰기 
                            }
                        }
                        SkinnedMeshRenderer tempSkinMesh = child.GetComponent<SkinnedMeshRenderer>();
                        if (tempSkinMesh != null)
                        {   Mesh tempMesh = tempSkinMesh.sharedMesh;
                            tmpVertexvector = tempMesh.vertices;

                            //정점 뽑기
                            if(tempMesh!=null)
                            sw.Write("Vertices: ");
                            sw.WriteLine(tempMesh.vertexCount);
                            for (int i = 0; i < tempMesh.vertexCount; ++i)
                            {
                                tvec[0] = tmpVertexvector[i].x;
                                tvec[1] = tmpVertexvector[i].y;
                                tvec[2] = tmpVertexvector[i].z;

                                sw.Write(tvec[0]);
                                sw.Write(" ");
                                sw.Write(tvec[1]);
                                sw.Write(" ");
                                sw.Write(tvec[2]);
                                sw.Write(" ");

                            }
                            //노말 뽑기
                            sw.WriteLine();
                            for (int i = 0; i < loopcnt; ++i)
                            {
                                sw.Write("           "); //들여쓰기 
                            }

                            sw.Write("Normals: ");
                            sw.WriteLine(tempMesh.vertexCount);   // 노말개수 인데 노말개수는 정점개수와 동일(Gunship.txt참고) 노멀 개수 참고하기 위해 작성
                            normalvector = tempMesh.normals;
                            for (int i = 0; i < tempMesh.vertexCount; ++i)
                            {
                                tvec[0] = normalvector[i].x;
                                tvec[1] = normalvector[i].y;
                                tvec[2] = normalvector[i].z;

                                sw.Write(tvec[0]);
                                sw.Write(" ");
                                sw.Write(tvec[1]);
                                sw.Write(" ");
                                sw.Write(tvec[2]);
                                sw.Write(" ");

                            }
                            sw.WriteLine();
                            for (int i = 0; i < loopcnt; ++i)
                            {
                                sw.Write("           "); //들여쓰기 
                            }

                            //testing
                            // index 뽑기 
                            sw.Write("Indices: ");

                            int submeshcnt = tempMesh.subMeshCount;

                            
                            if (submeshcnt != 0)
                            {
                                uint meshindexcount = tempMesh.GetIndexCount(tempMesh.subMeshCount);
                                sw.Write(meshindexcount);


                                sw.Write(submeshcnt);
                                //uint indexccnt = tempMesh.GetIndexCount(submeshcnt);
                                //sw.Write(indexccnt);
                            }

                            //int submeshcnt =  tempMesh.subMeshCount;
                            // sw.Write(submeshcnt);
                            //tempMesh.GetIndexCount(subMeshCnt);

                            //int[] tempIndicies = tempMesh.GetIndices(0);
                            //sw.Write(tempIndicies[0]);

                            //int[] getindicies = tempMesh.GetIndices(submeshcnt);

                        }
                        sw.WriteLine(); // 줄바꿈
                        //sw.Write("           "); //들여쓰기 
                        loopcnt++;
                    }
                }

                ////정점 뽑기
                //sw.WriteLine("Vertices: ");
                //for(int i =0; i< testMesh.vertexCount;++i)
                //{
                //    tvec[0] = vertexvector[i].x;
                //    tvec[1] = vertexvector[i].y;
                //    tvec[2] = vertexvector[i].z;

                //    sw.Write(tvec[0]);
                //    sw.Write(" ");
                //    sw.Write(tvec[1]);
                //    sw.Write(" ");
                //    sw.Write(tvec[2]);
                //    sw.Write(" ");
                //    //sw.WriteLine();
                //}
                //sw.WriteLine(testx);
                
                //Write a second line of text
                //sw.WriteLine("From the StreamWriter class");

                //close the file
                sw.Close();
            }
            catch
            {

            }
            finally
            {
                
            }
        }

        // Update is called once per frame
        void Update()
        {

        }
    }
}
