using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class BrakeBlock : interfaceIGimmick
{
    [SerializeField] private GameObject block;       // �j�󂷂�u���b�N
    [SerializeField] private AudioClip se;
    [SerializeField] private Animator animator;
    AudioSource audiosource;
    [SerializeField] private ParticleSystem particle;
    ParticleSystem newparticle;
    void Start()
    {
        newparticle = Instantiate(particle);
        audiosource = GetComponent<AudioSource>();
    }
    public override void IActlable()
    {
        // �G�t�F�N�g�Đ�
        newparticle.transform.position = this.transform.position;
        newparticle.Play();
        AudioSource.PlayClipAtPoint(se,transform.position);
        Debug.Log("�j��");
        Destroy(block);
    }
    // Update is called once per frame

    private void Update()
    {
        /*if (Keyboard.current.enterKey.wasPressedThisFrame)
        {
            IActlable();
        }
        */
    }

    private void OnCollisionEnter(Collision other)
    {
        if (animator.GetCurrentAnimatorStateInfo(0).IsName("animation"))
        {
            if ((GameObject.FindGameObjectWithTag("player").GetComponent<PlayerMovement>()._playerdir == true && GameObject.FindGameObjectWithTag("player").transform.position.x < transform.position.x) || (GameObject.FindGameObjectWithTag("player").GetComponent<PlayerMovement>()._playerdir == false && GameObject.FindGameObjectWithTag("player").transform.position.x > transform.position.x))
            {
                if (other.gameObject.tag == "hammer")
                {

                    IActlable();
                }
            }
        }
    }
}
