using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using System.IO;
using UniRx;

public class BreakScreenManager : MonoBehaviour
{
    public RenderTexture renderTexture;
    private Camera renderCamera;
    [SerializeField] private Camera renderecamera;
    private Camera newcamera;
    public ReactiveProperty<bool> breakscreen = new ReactiveProperty<bool>();
    public static BreakScreenManager instance;
    [SerializeField] private BreakScreen BreakScreen;
    private BreakScreen bs;
    [SerializeField] private GameObject breakback;
    private GameObject back;

    private void Start()
    {
        if (instance == null)
        {
            instance = this;
        }
        else
        {
            Destroy(this.gameObject);
        }
        //renderCamera = Camera.main.transform.GetChild(0).GetComponent<Camera>();
        breakscreen.Value = false;
        // DoTrace();
    }

    private void Update()
    {
        //if (Keyboard.current.tabKey.wasPressedThisFrame)
        //{
        //    DoTrace();
        //}

    }

    public void DoTrace()
    {
        renderCamera = GameObject.Find("Main Camera").GetComponent<Camera>();
        if(newcamera == null)
        {
            newcamera = Instantiate(renderecamera);
            newcamera.transform.parent = renderCamera.transform;
            newcamera.transform.localPosition = new Vector3(0.0f, 0.0f, 0.0f);
            StartCoroutine("Trace");
        }
    }

    IEnumerator Trace()
    {
        //Debug.Log("aaaa");
        Texture2D tex = new Texture2D(renderTexture.width, renderTexture.height, TextureFormat.RGB24, false);
        RenderTexture.active = renderTexture;
        tex.ReadPixels(new Rect(0, 0, renderTexture.width, renderTexture.height), 0, 0);
        if (QualitySettings.activeColorSpace == ColorSpace.Linear)
        {
            // カメラの映像が暗くなってしまうのでガンマ補正をかける
            var color = tex.GetPixels();
            for (int i = 0; i < color.Length; i++)
            {
                color[i].r = Mathf.LinearToGammaSpace(color[i].r);
                color[i].g = Mathf.LinearToGammaSpace(color[i].g);
                color[i].b = Mathf.LinearToGammaSpace(color[i].b);
            }
            tex.SetPixels(color);
        }
        tex.Apply();

        byte[] bytes = tex.EncodeToPNG();
        //BreakImage.instance.SetImage(bytes);

        //File.WriteAllBytes("RenderTexture.png", bytes);

        newcamera.enabled = true;
        newcamera.targetTexture = renderTexture;
        renderTexture.Release();
        yield return null;

        if(bs == null)
        {
            bs = Instantiate(BreakScreen);         // 割れスクリーン生成
        }
        back = Instantiate(breakback);
        back.transform.parent = bs.transform;

        newcamera.targetTexture = null;
        breakscreen.Value = true;               // スクリーン破壊を有効化
        newcamera.enabled = false;
        //breakscreen.Value = false;

    }

    public void SetImage(byte[] im)
    {
        //File.WriteAllBytes("RenderTexture.png", im);
    }

    public void DoBreak()
    {
        breakscreen.Value = true;
    }

    public void ResetBreak()
    {
        breakscreen.Value = false;
    }
}
