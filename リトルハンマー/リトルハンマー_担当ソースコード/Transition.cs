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

    // �g�����W�V��������(�b���w��)
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
    // �g�����W�V��������
    public void DoTransition()
    {
        if (!coroutineBool)
        {
            SetEdge(0.02f);
            coroutineBool = true;
            StartCoroutine("TransitionFade");
        }
    }
    // �g�����W�V��������(�b���A�V�[���J�ڎw��)
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

    // �g�����W�V��������(�V�[���J�ڎw��)
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
        // �g�����W�V���������̐i�s����Z�b�g(1�������������)
        material.SetFloat("_Progress",progress);
    }
    public void SetEdge(float edge)
    {
        // �֊s�̃{�P����Z�b�g(����:0�`0.02���炢)
        material.SetFloat("_Edge", edge);
    }

    // �g�����W�V��������(�R���[�`��)
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
    // �g�����W�V�����V�[���J�ڏ���(�R���[�`��)
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
        SceneManager.LoadScene(name);      // �O������w�肵���V�[���������[�h
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
