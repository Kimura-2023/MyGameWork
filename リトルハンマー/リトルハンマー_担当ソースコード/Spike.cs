using System.Collections;
using System.Collections.Generic;
using UnityEngine.SceneManagement;
using UnityEngine;
using UnityEngine.InputSystem;

public class Spike : interfaceIGimmick
{
    [SerializeField] private AudioClip se;
    AudioSource audiosource;
    private void Start()
    {
        audiosource = GetComponent<AudioSource>();
    }
    public override void IActlable()
    {
        Debug.Log("トゲ");
        Scene loadScene = SceneManager.GetActiveScene();
        //SceneManager.LoadScene(loadScene.name);
        //Fade.instance.SceneFade(loadScene.name);
        //Fade.instance.Transition();
        Fade.instance.FadeOut(0.9f);
        GameOver.instance.Miss();
        //Transition.instance.DoTransition(loadScene.name);
    }

    // Update is called once per frame
    void Update()
    {
        // デバッグ用
        //if (Keyboard.current.tabKey.wasPressedThisFrame)
        //{
        //    IActlable();
        //}
        //if(Keyboard.current.spaceKey.wasPressedThisFrame)
        //{
        //    Fade.instance.FadeIn();
        //}
    }

    private void OnCollisionEnter(Collision other)
    {
        if (other.gameObject.tag == "player")
        {
            IActlable();
        }
    }
}
