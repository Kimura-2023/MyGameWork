using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class Transition : MonoBehaviour
{
    private Material material;
    public static Transition instance;
    private float speed;
    private float limit;
    private float time;
    private bool coroutineBool;

    // Start is called before the first frame update
    void Start()
    {
        if (instance == null)
        {
            instance = this;
        }
        else
        {
            Destroy(this.gameObject);
        }
        material = GetComponent<Image>().material;
        material.EnableKeyword("_Progress");
        material.EnableKeyword("_Edge");

        material.SetFloat("_Edge", 0.0f);
        material.SetFloat("_Progress", 1.0f);

        speed = 0.1f;
        limit = 1.0f;
        time = 100.0f;
        coroutineBool = false;
    }

    // トランジション処理(秒数指定)
    public void DoTransition(float seconds)
    {
        if(!coroutineBool)
        {
            coroutineBool = true;
            time = seconds * 100.0f;
            SetEdge(0.02f);
            StartCoroutine("TransitionFade");
        }
    }
    // トランジション処理
    public void DoTransition()
    {
        if (!coroutineBool)
        {
            SetEdge(0.02f);
            coroutineBool = true;
            StartCoroutine("TransitionFade");
        }
    }
    // トランジション処理(秒数、シーン遷移指定)
    public void DoTransition(float seconds,string name)
    {
        if (!coroutineBool)
        {
            time = seconds * 100.0f;
            SetEdge(0.02f);
            coroutineBool = true;
            StartCoroutine(TransitionFade(name));
        }
    }

    // トランジション処理(シーン遷移指定)
    public void DoTransition(string name)
    {
        if (!coroutineBool)
        {
            SetEdge(0.02f);
            coroutineBool = true;
            StartCoroutine(TransitionFade(name));
        }
    }

    public void SetLimit(float lim)
    {
        limit = lim;
    }

    public void SetProgress(float progress)
    {
        // トランジション処理の進行具合をセット(1が何も無い状態)
        material.SetFloat("_Progress",progress);
    }
    public void SetEdge(float edge)
    {
        // 輪郭のボケ具合をセット(推奨:0～0.02くらい)
        material.SetFloat("_Edge", edge);
    }

    // トランジション処理(コルーチン)
    IEnumerator TransitionFade()
    {
        for(int i = 0;i<time;i++)
        {
            if(i>time)
            {
                yield break;
            }
            material.SetFloat("_Edge", material.GetFloat("_Edge") + 0.01f);
            material.SetFloat("_Progress", material.GetFloat("_Progress") - (1.0f/time));
            yield return new WaitForSeconds(0.01f);
        }
        coroutineBool = false;
    }
    // トランジションシーン遷移処理(コルーチン)
    IEnumerator TransitionFade(string name)
    {
        for (int i = 0; i < time; i++)
        {
            if (i > time)
            {
                yield break;
            }
            material.SetFloat("_Edge", material.GetFloat("_Edge") + 0.01f);
            material.SetFloat("_Progress", material.GetFloat("_Progress") - (1.0f / time));
            yield return new WaitForSeconds(0.01f);
        }
        coroutineBool = false;
        Fade.instance.FadeIn();
        MissText.instance.Reset();
        StartCoroutine("ResetTransitionFade");
        GameOver.instance.Restart();
        SceneManager.LoadScene(name);      // 外部から指定したシーン名をロード
    }
    IEnumerator ResetTransitionFade()
    {
        coroutineBool = true;
        for (int i = 0; i < time; i++)
        {
            if (i > time)
            {
                yield break;
            }
            material.SetFloat("_Edge", material.GetFloat("_Edge") - 0.01f);
            material.SetFloat("_Progress", material.GetFloat("_Progress") + (1.0f / time));
            yield return new WaitForSeconds(0.01f);
        }
        SetEdge(0.0f);
        coroutineBool = false;
    }
    public bool GetTransition()
    {
        return coroutineBool;
    }
}
