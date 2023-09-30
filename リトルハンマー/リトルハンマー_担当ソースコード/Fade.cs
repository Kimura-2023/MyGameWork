using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class Fade : MonoBehaviour
{
    public static Fade instance;
    [SerializeField] private float speed = 0.01f;
    private float limit = 1.0f;                 // 目標値
    private float alpha = 1.0f;                 // α値
    private string scenename;                   // 切り替えるシーン名
    float red, green, blue;                     // RGB
    bool fadeout = false;                       // フェードアウト
    bool fadein = false;                        // フェードイン
    bool isFade = false;                        // フェード中
    bool SceneChange = false;                   // シーン切り替えるか
    private Material material;

    // Start is called before the first frame update
    void Start()
    {
        if (instance == null)
        {
            GetComponentInParent<Canvas>().gameObject.AddComponent<DontDestroy>();
            instance = this;
        }
        else
        {
            Destroy(this.gameObject);
        }

        red = GetComponent<Image>().color.r;
        green = GetComponent<Image>().color.g;
        blue = GetComponent<Image>().color.b;
        FadeIn();
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        // ===フェードアウト===
        if (fadeout == true && isFade == true)
        {
            GetComponent<Image>().color = new Color(red, green, blue, alpha);
            if (alpha > limit)
            {
                fadeout = false;
                isFade = false;
                // ---シーン切り替えする場合
                if(SceneChange)
                {
                    SceneManager.LoadScene(scenename);      // 外部から指定したシーン名をロード
                    SceneChange = false;
                }
            }
            else
            {
                alpha += speed;

            }
        }

        // ===フェードイン===
        if (fadein == true && isFade == true)
        {
            GetComponent<Image>().color = new Color(red, green, blue, alpha);
            alpha -= speed;

            if (alpha < 0)
            {
                fadein = false;
                isFade = false;
            }
        }
    }

    // ===フェードイン(外から呼ぶ用)
    public void FadeIn()
    {
        // フェード中の場合リターン
        if (isFade)
        {
            return;
        }
        alpha = 1.0f;
        fadeout = false;
        fadein = true;
        isFade = true;
    }

    // ===フェードアウト(外から呼ぶ用)
    public void FadeOut()
    {
        // フェード中の場合リターン
        if (isFade)
        {
            return;
        }
        fadeout = true;
        fadein = false;
        isFade = true;
        limit = 1.0f;
    }

    // ===フェードアウト(進行度指定)
    public void FadeOut(float alpha)
    {
        // フェード中の場合リターン
        if (isFade)
        {
            return;
        }
        fadeout = true;
        fadein = false;
        isFade = true;
        limit = alpha;
    }

    // ===フェード状況取得===
    public bool GetFade()
    {
        return isFade;          // フェード中true
    }

    // ===フェード後シーン切り替え===
    // 引数:.Sceneの名前
    // ==============================
    public void SceneFade(string name)
    {
        // フェード中は処理をしない
        if(isFade)
        {
            return;
        }
        fadeout = true;
        fadein = false;
        isFade = true;

        scenename = name;
        SceneChange = true;
    }
}
