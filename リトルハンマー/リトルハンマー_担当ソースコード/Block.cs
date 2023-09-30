using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using UniRx;
using System;

[RequireComponent(typeof(Rigidbody))]
public class Block : interfaceIGimmick
{
   
    [SerializeField] private GameObject block;       // �������u���b�N
    public BlockStatus blockStatus = new BlockStatus();
    [SerializeField] private MoveCollision moveCollision;
    [SerializeField] private AudioClip se;
    [SerializeField] private Animator animator;
    [SerializeField] private ParticleSystem moveEffect;
    [SerializeField] private ParticleSystem GroundEffect;
    //private bool flg = false;
    AudioSource audiosource;
    private float distance;
    public float dir;
    public Subject<uint> stopSubject = new Subject<uint>();
    ParticleSystem newparticle;
    ParticleSystem newparticle2;


    void Start()
    {
        moveCollision.MoveStop.Where(x => x == true).Subscribe(_ => Stop());
        moveCollision.Move.Where(x => x == true).Subscribe(_ => MoveBlock());
        audiosource = GetComponent<AudioSource>();
        distance = 1.0f;
        dir = 1.0f;

        newparticle = Instantiate(moveEffect);
        newparticle2 = Instantiate(GroundEffect);
    }

    public IObservable<uint> Observable
    {
        get { return stopSubject; }
    }

    public override void IActlable()
    {
        if (blockStatus.OnWallFace)
        {
            return;
        }
        // �u���b�N�̋���
        Debug.Log("�ړ�");
        blockStatus.IsImpact = true;
       

    }

    private void Update()
    {
        // �f�o�b�O�p
        //if (Keyboard.current.spaceKey.wasPressedThisFrame)
        //{
        //    IActlable();
        //}
    }

    private void FixedUpdate()
    {
        // ===�ړ��u���b�N���@����ĕǂɂԂ����ĂȂ��ꍇ
        if(blockStatus.IsImpact == true && blockStatus.OnWallFace == false)
        {
            Vector3 EfcPos = this.transform.position;
            this.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.None;
            this.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezeRotation | RigidbodyConstraints.FreezePositionZ;

            // ===�ړ�===
            gameObject.transform.position += new Vector3(0.5f * dir, 0.0f, 0.0f);

            this.GetComponent<Rigidbody>().isKinematic = false;
            this.GetComponent<Rigidbody>().useGravity = false;

            // === �n�ʂƐڒn���Ă��Ȃ��ꍇ===
            if (!CheckCollision())
            {
                gameObject.transform.position += new Vector3(0.0f, -0.5f / 7.0f, 0.0f);
            }

            // ===�G�t�F�N�g�Đ�===
            EfcPos.x = EfcPos.x - transform.localScale.x / 2 * dir;
            EfcPos.z = EfcPos.z - transform.localScale.z / 2;


            newparticle.transform.position = EfcPos;
            newparticle.Play();

            if (CheckCollision())
            {
                EfcPos = this.transform.position;
                EfcPos.x = EfcPos.x - (transform.localScale.x / 2 * dir);
                EfcPos.y = EfcPos.y - transform.localScale.y / 2;

                newparticle2.transform.position = EfcPos;
                newparticle2.Play();
            }
        }

        // ===�u���b�N�������Ă��Ȃ��Ƃ�===
        if(blockStatus.IsImpact == false)
        {
            // �n�ʂƐڒn���Ă��Ȃ�
            if (!CheckCollision())
            {
                this.GetComponent<Rigidbody>().isKinematic = false;
                this.GetComponent<Rigidbody>().useGravity = true;
            }
            // �n�ʂƐڒn���Ă���
            if (CheckCollision())
            {
                //this.transform.position = new Vector3(this.transform.position.x, Mathf.RoundToInt(this.transform.position.y), this.transform.position.z);
                this.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezeRotation | RigidbodyConstraints.FreezePositionZ | RigidbodyConstraints.FreezePositionX;
                this.GetComponent<Rigidbody>().isKinematic = true;
                this.GetComponent<Rigidbody>().useGravity = false;
            }
        }
    }

    // ===�n���}�[�Ɠ�������
    private void OnCollisionEnter(Collision collision)
    {
        // ===�A�j���[�V�����Đ���
        if (!GameObject.FindGameObjectWithTag("player"))
            return;
        if ((GameObject.FindGameObjectWithTag("player").transform.position.x > transform.position.x && GameObject.FindGameObjectWithTag("player").GetComponent<PlayerMovement>()._playerdir == false) || (GameObject.FindGameObjectWithTag("player").transform.position.x < transform.position.x && GameObject.FindGameObjectWithTag("player").GetComponent<PlayerMovement>()._playerdir == true))
        { 
            if (animator.GetCurrentAnimatorStateInfo(0).IsName("animation"))
            {
                if (collision.gameObject.tag == "hammer")
                {
                    IActlable();
                    if (GameObject.FindGameObjectWithTag("player").transform.position.x > transform.position.x)
                    {
                        dir = -1.0f;
                    }
                    if (GameObject.FindGameObjectWithTag("player").transform.position.x < transform.position.x)
                    {
                        dir = 1.0f;
                    }
                }
            }
        }
    }

    // �n�ʂƂ̐ڒn����(raycast)
    private bool CheckCollision()
    {
        Vector3 rayRPosition = transform.position + new Vector3(transform.localScale.x / 2 - 0.5f, 0.0f, 0.0f);
        Vector3 rayLPosition = transform.position + new Vector3(-transform.localScale.x / 2 + 0.5f, 0.0f, 0.0f);
        Ray Rray = new Ray(rayRPosition, Vector3.down);
        Ray Lray = new Ray(rayRPosition, Vector3.down);
        bool isGround = false;
        if(Physics.Raycast(Rray, (distance * transform.localScale.y / 2)+0.001f) || Physics.Raycast(Rray, (distance * transform.localScale.y / 2) + 0.001f))
        {
            isGround = true;
        }
        Debug.DrawRay(rayRPosition, Vector3.down * distance * transform.localScale.y / 2, Color.red);
        Debug.DrawRay(rayLPosition, Vector3.down * distance * transform.localScale.y / 2, Color.red);

        //Debug.Log(isGround);
        return isGround;
    }

    // ===�ǂƏՓ�
    public void Stop()
    {
        // ===SE�Đ�
        if(blockStatus.IsImpact == true)
        { 
            if (!audiosource.isPlaying)
            {
                audiosource.PlayOneShot(se);
            }
            stopSubject.OnNext(1);

        }

        this.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezeRotation | RigidbodyConstraints.FreezePositionZ ;
        this.GetComponent<Rigidbody>().isKinematic = false;
        blockStatus.OnWallFace = true;
        blockStatus.IsImpact = false;
        Debug.Log("�~�܂�");
    }

    public void MoveBlock()
    {
        this.GetComponent<Rigidbody>().useGravity = true;
        blockStatus.OnWallFace = false;
    }

}