using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UniRx;
using System;
public class GameClear : MonoBehaviour
{
    [SerializeField] private OnGoal ongoal;
    public AudioClip sound1;
    AudioSource audioSource;
    AudioSource aBGM;
    [System.NonSerialized] public ReactiveProperty<float> bgmvolume = new ReactiveProperty<float>(1);
    Camera mainCamera;

    // Start is called before the first frame update
    void Start()
    {
        //Component‚ðŽæ“¾
        audioSource = gameObject.AddComponent<AudioSource>();
        mainCamera = Camera.main;
        aBGM = mainCamera.GetComponent<AudioSource>();
        ongoal.Ongoal.Subscribe(onNext: _ => { aBGM.Stop(); audioSource.PlayOneShot(sound1); });
    }

    // Update is called once per frame
    void Update()
    {

    }

}
