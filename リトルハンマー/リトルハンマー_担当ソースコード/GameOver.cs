using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UniRx;

public class GameOver : MonoBehaviour
{
    public static GameOver instance;
    public ReactiveProperty<bool> gameover = new ReactiveProperty<bool>();
    private AudioSource audiosource;
    [SerializeField] private AudioClip se;
    private bool isGameover;

    void Start()
    {
        if (instance == null)
        {
            instance = this;
        }
        else
        {
            Destroy(this.gameObject);
            return;
        }
        gameover.Value = false;
        isGameover = false;
        audiosource = gameObject.AddComponent<AudioSource>();
    }
    public void Miss()
    {
        gameover.Value = true;
        isGameover = true;
        if (!audiosource.isPlaying)
        {
            audiosource.PlayOneShot(se);
        }
    }
    public void Restart()
    {
        gameover.Value = false;
        isGameover = false;
    }

    public bool GetGameover()
    {
        return isGameover;
    }

}
