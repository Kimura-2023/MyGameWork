using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class Fade : MonoBehaviour
{
    public static Fade instance;
    [SerializeField] private float speed = 0.01f;
    private float limit = 1.0f;                 // �ڕW�l
    private float alpha = 1.0f;                 // ���l
    private string scenename;                   // �؂�ւ���V�[����
    float red, green, blue;                     // RGB
    bool fadeout = false;                       // �t�F�[�h�A�E�g
    bool fadein = false;                        // �t�F�[�h�C��
    bool isFade = false;                        // �t�F�[�h��
    bool SceneChange = false;                   // �V�[���؂�ւ��邩
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
        // ===�t�F�[�h�A�E�g===
        if (fadeout == true && isFade == true)
        {
            GetComponent<Image>().color = new Color(red, green, blue, alpha);
            if (alpha > limit)
            {
                fadeout = false;
                isFade = false;
                // ---�V�[���؂�ւ�����ꍇ
                if(SceneChange)
                {
                    SceneManager.LoadScene(scenename);      // �O������w�肵���V�[���������[�h
                    SceneChange = false;
                }
            }
            else
            {
                alpha += speed;

            }
        }

        // ===�t�F�[�h�C��===
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

    // ===�t�F�[�h�C��(�O����Ăԗp)
    public void FadeIn()
    {
        // �t�F�[�h���̏ꍇ���^�[��
        if (isFade)
        {
            return;
        }
        alpha = 1.0f;
        fadeout = false;
        fadein = true;
        isFade = true;
    }

    // ===�t�F�[�h�A�E�g(�O����Ăԗp)
    public void FadeOut()
    {
        // �t�F�[�h���̏ꍇ���^�[��
        if (isFade)
        {
            return;
        }
        fadeout = true;
        fadein = false;
        isFade = true;
        limit = 1.0f;
    }

    // ===�t�F�[�h�A�E�g(�i�s�x�w��)
    public void FadeOut(float alpha)
    {
        // �t�F�[�h���̏ꍇ���^�[��
        if (isFade)
        {
            return;
        }
        fadeout = true;
        fadein = false;
        isFade = true;
        limit = alpha;
    }

    // ===�t�F�[�h�󋵎擾===
    public bool GetFade()
    {
        return isFade;          // �t�F�[�h��true
    }

    // ===�t�F�[�h��V�[���؂�ւ�===
    // ����:.Scene�̖��O
    // ==============================
    public void SceneFade(string name)
    {
        // �t�F�[�h���͏��������Ȃ�
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
